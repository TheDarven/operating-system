#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define NBQUEUE 10

#include "scheduler.h"

typedef struct MessageFile {
    int* messages;                          // Tête de la file
    int nbMaxMessage;
    int nbMessage;                          // Nombre de messages actuellement stockés dans la file

    int indexFirst;
    int indexLast;

    link blockedConsumerQueueHead;   // Queue des consommateurs en attente

    link blockedProducerQueueHead; // Queue des producteurs en attente
    
    int nbConsumer;         // Nombre de processus bloqués sur 'file pleine'
    int nbProducer;          // Nombre de processus bloqués sur 'file vide'

} MessageFile;

extern MessageFile* fileList[NBQUEUE];

Process* pickConsumer(MessageFile* file);

Process* pickProducer(MessageFile* file);

void removeFromMessageFile(Process* process);

void addConsumer(MessageFile* file, Process* process);

void addProducer(MessageFile* file, Process* process);

void removeAllProducer(MessageFile* file);

void removeAllConsumer(MessageFile* file);



int pcreate(int count);

int pdelete(int fid);

int pcount(int fid, int *count);

int psend(int fid, int message);

int preceive(int fid, int *message);

int preset(int fid);

int deleteLastChar(int fid);

#endif