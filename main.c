//
// Created by jp on 12/21/16.
//

#include <unistd.h>
#include "main.h"



struct device dev;
char buf[sizeof(struct message)];
int main(int argc,char *argv[]){

    if(!strcmp(argv[1],"--help")){
        printf("help\n");
    }
    else if (argc < 3){/////attention client
        printf("Please add IP argument:my-address port socket-mode[server/client] their-address port(client)\n");
        exit(1);
    }





    pthread_t circleThread,commandThread;
    pthread_create(&circleThread,NULL,(void *)circleSync,NULL);
    while(1){
            sleep(500);
    }
    //pthread_create(&commandThread,NULL,(void *)listenCommand,argv[0]);
    //sendCommand(argv[0]);
}
