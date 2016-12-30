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


    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(argv[1]);
    addr.sin_port=htons(atoi(argv[2]));
    dev.addr=addr;

    if (strcasecmp(argv[3],"server")==0) {
        dev.character=host;//////////tempral host decide, server is host
        dev.state=wait_send;
    }else{
        dev.character=slave;
        dev.state=wait_recive;
        dev.their_addr.sin_family=AF_INET;
        dev.their_addr.sin_addr.s_addr=inet_addr(argv[4]);
        dev.their_addr.sin_port=htons(atoi(argv[5]));

    }
    getSocket();
    pthread_t circleThread,commandThread;
    pthread_create(&circleThread,NULL,(void *)circleSync,NULL);
    while(1){
            sleep(500);
    }
    //pthread_create(&commandThread,NULL,(void *)listenCommand,argv[0]);
    //sendCommand(argv[0]);
}
