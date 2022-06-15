#include "message.h"

MessageFile* fileList[NBQUEUE];

Process* pickConsumer(MessageFile* file) {
    if (file->nbConsumer == 0 || queue_empty(&(file->blockedConsumerQueueHead)) != 0) {
        return NULL;
    }

    file->nbConsumer--;

    return queue_out(&(file->blockedConsumerQueueHead), Process, messageQueue);
}

Process* pickProducer(MessageFile* file) {
    if (file->nbProducer == 0 || queue_empty(&(file->blockedProducerQueueHead)) != 0) {
        return NULL;
    }

    file->nbProducer--;

    return queue_out(&(file->blockedProducerQueueHead), Process, messageQueue);
}


void addConsumer(MessageFile* file, Process* process) {
    if (file->nbMessage > 0) {
        return;
    }
    
    process->waitMessageFile = true;

    queue_add(process, &(file->blockedConsumerQueueHead), Process, messageQueue, priority); 

    file->nbConsumer++;
}

void addProducer(MessageFile* file, Process* process) {
    if (file->nbMessage < file->nbMaxMessage) {
        return;
    }
    
    process->waitMessageFile = true;
    
    queue_add(process, &(file->blockedProducerQueueHead), Process, messageQueue, priority); 
    
    file->nbProducer++;
}

void removeAllProducer(MessageFile* file) {
    while (queue_empty(&(file->blockedProducerQueueHead)) == 0) {
        Process* producerProcess = pickProducer(file);

        producerProcess->waitMessageFile = false;

        switchState(producerProcess, READY);
    }
}

void removeAllConsumer(MessageFile* file) {
    while (queue_empty(&(file->blockedConsumerQueueHead)) == 0) {
        Process* consumerProcess = pickConsumer(file);

        consumerProcess->waitMessageFile = false;

        switchState(consumerProcess, READY);
    }
}




int pcreate(int count) {

    if (count < 1) {
        return -1;
    }
    
    int fileIndex = 0;
    for (; fileIndex < NBQUEUE; fileIndex++) {
        if (fileList[fileIndex] == NULL) {
            break;
        }
    }

    if (fileIndex == NBQUEUE) {
        return -1;
    }

    MessageFile* newMessageFile = (MessageFile*) mem_alloc(sizeof(MessageFile));

    fileList[fileIndex] = newMessageFile;

    newMessageFile->nbMaxMessage = count;

    newMessageFile->nbMessage = 0;

    newMessageFile->indexFirst = 0;

    newMessageFile->indexLast = count - 1;

    newMessageFile->messages = (int*) mem_alloc(sizeof(int) * count);

    newMessageFile->blockedProducerQueueHead = (link) LIST_HEAD_INIT(newMessageFile->blockedProducerQueueHead);
    
    newMessageFile->blockedConsumerQueueHead = (link) LIST_HEAD_INIT(newMessageFile->blockedConsumerQueueHead);

    return fileIndex;
}

int pdelete(int fid) {
    if (fid < 0 || fid >= NBQUEUE) {
        return -1;
    }

    MessageFile* targetFile = fileList[fid];
    if (targetFile == NULL) {
        return -1;
    }

    removeAllConsumer(targetFile);

    removeAllProducer(targetFile);

    mem_free(targetFile->messages, sizeof(int) * targetFile->nbMaxMessage);

    mem_free(targetFile, sizeof(MessageFile));

    return 0;
}



// Obtention d'informations sur une file de messages
int pcount(int fid, int *count) {
    // Si la valeur de fid est invalide, pcount signale l'erreur en retournant un code de retour négatif, sinon 0.
    if (fid < 0 || fid >= NBQUEUE) {
        return -1;
    }

    // Récupère la file de messages :
    MessageFile* targetFile = fileList[fid];
    if (targetFile == NULL) {
        return -1;
    }

    if (count != NULL) {

        // Place dans 'count' une valeur négative égale à l'opposé du nombre de processus bloqués sur file vide :
        if (targetFile->nbConsumer > 0) {
            *count = - targetFile->nbConsumer;
        }
        // Place dans 'count' une valeur positive égale à la somme du nombre de messages dans la file et du nombre de processus bloqués sur file pleine.
        else {
            *count = targetFile->nbMessage + targetFile->nbProducer;
        }
    }

    return 0;
}


int psend(int fid, int message) {

    // Fid validity check
    if (fid < 0 || fid >= NBQUEUE) {
        return -1;
    }

    // Récupère la file de messages :
    MessageFile* file = fileList[fid];
    if (file == NULL) {
        return -1;
    }

    // File de message pleine
    if (file->nbMessage == file->nbMaxMessage) {
        addProducer(file, runningProcess);

        switchState(runningProcess, BLOCKED_MSG);

        ordonnance();

        if (runningProcess->waitMessageFile) {
            runningProcess->waitMessageFile = false;
        } else {
            // La file a été supprimée
            return -1;
        }
    }

    file->nbMessage++;

    file->indexLast = (file->indexLast + 1) % file->nbMaxMessage;

    file->messages[file->indexLast] = message;

    // Si des processus attendent de consommer un message, on en réveille un
    Process* wakeConsumer = pickConsumer(file);
    if (wakeConsumer != NULL) {
        switchState(wakeConsumer, READY);
    }
    return 0;
}


// Réception d'un message sur une file
int preceive(int fid, int *message) {

    // Si la valeur de fid n'est pas appropriée
    if (fid < 0 || fid >= NBQUEUE) {
        return -1;
    }

    // Récupère la file de messages :
    MessageFile* file = fileList[fid];
    if (file == NULL) {
        return -1;
    }

    // Si on n'a pas de message, le processus se bloque :
    if (file->nbMessage == 0) {
        addConsumer(file, runningProcess);

        switchState(runningProcess, BLOCKED_MSG);

        ordonnance();

        if (runningProcess->waitMessageFile) {
            runningProcess->waitMessageFile = false;
        } else {
            return -1; // La file a été supprimée
        }
    }

    // Lecture du message :
    if (file->messages[file->indexFirst] != 0 && message != NULL) {
        *message = file->messages[file->indexFirst];
    }

    // Mise à jour du nombre de messages de la file et du premier index de la file :
    file->indexFirst = (file->indexFirst + 1) % file->nbMaxMessage;
    file->nbMessage--;

    // Si des processus attendent de produire un message, on en réveille un
    Process* waitProducer = pickProducer(file);
    if (waitProducer != NULL) {
        switchState(waitProducer, READY);
    }

    return 0;
    
}

int preset(int fid) {
    
    // Si la valeur de fid n'est pas appropriée
    if (fid < 0 || fid >= NBQUEUE) {
        return -1;
    }

    // Récupère la file de messages :
    MessageFile* file = fileList[fid];
    if (file == NULL) {
        return -1;
    }

    file->nbMessage = 0;
    
    file->indexFirst = 0;
    file->indexLast = file->nbMaxMessage - 1;

    removeAllConsumer(file);

    removeAllProducer(file);

    return 0;
}
