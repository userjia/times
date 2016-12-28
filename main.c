//
// Created by jp on 12/21/16.
//
#include "main.h"



struct device dev;
char buf[sizeof(struct message)];
int main(int argc,char *argv[]){

    if(!strcmp(argv[1],"--help")){
        printf("help\n");
    }
    else if (argc < 2){
        printf("Please add IP argument:dest-address(client) self-address port socket-mode(server/client)\n");
        exit(1);
    }


    char c[2]="y";
    while(strcasecmp(c,"y")==0){

        if (strcasecmp(argv[4],"server")==0) {
            dev.character=host;//////////tempral host decide, server is host
            dev.state=wait_send;
            dev.addr=argv[2];
            
            getSocket("server");
        }else{
            dev.character=slave;
            dev.state=wait_recive;
            getSocket("client");
        }

        if (dev.fd!=-1){
            printf("Connect success.\n");
            break;
        } else{
            printf("Connect fail.\n");
            printf("Try again?(y) Quit?(n):");
            scanf("%s",c);
            if(strcasecmp(c,"n")){
                exit(-2);
            }
        }
    }

    pthread_t circleThread,commandThread;
    pthread_create(&circleThread,NULL,(void *)circleSync,NULL);
    while(1){

    }
    //pthread_create(&commandThread,NULL,(void *)listenCommand,argv[0]);
    //sendCommand(argv[0]);
}
