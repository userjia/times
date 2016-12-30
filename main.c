//
// Created by jp on 12/21/16.
//

#include <unistd.h>
#include "main.h"



struct device dev;
char buf[sizeof(struct message)];
int main(int argc,char *argv[]){
    configSocket();
    getSocket();
    pthread_t circleThread,commandThread;
    pthread_create(&circleThread,NULL,(void *)circleSync,NULL);
    while(1){
            sleep(500);
    }
    //pthread_create(&commandThread,NULL,(void *)listenCommand,argv[0]);
    //sendCommand(argv[0]);
}
