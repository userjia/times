//
// Created by jp on 12/21/16.
//

#include <unistd.h>
#include "main.h"

struct device dev;
int main(int argc,char *argv[]){
    configSocket();
    getSocket();
    pthread_t circleThread;
    pthread_create(&circleThread,NULL,(void *)circleSync,NULL);
    listenCommand(argv[0]);
}
