//
// Created by jp on 12/21/16.
//

#include <time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "entity.h"


struct device dev;

long int out(enum messageState ms){
    struct message msg;
    msg.messageType=ms;
    long int len=send(dev.fd,&msg,sizeof(msg),0);
    return len;
}

struct message *receive(enum messageState ms){
    struct message *msg;
    msg=(struct message *)malloc(sizeof(struct message));
    long int len=recv(dev.fd,msg,sizeof(*msg),0);
    if (len!=-1) {
        return msg;
    }else{
        return NULL;
    }

}

char *getTime(){
    time_t t;
    struct tm *localTime;
    t= time(NULL);
    localTime=localtime(&t);
    return asctime(localTime);
}


int getSocket(char *input_addr,char *port,char *socketType){
    int sockfd,fd;
    int lisnum=1;//for listening client extend
    struct sockaddr_in addr,their_addr;
    socklen_t socklen;

    bzero(&addr, sizeof(addr));//TODO what is bzero,htons?
    addr.sin_family=PF_INET;
    addr.sin_port=htons(atoi(port));
    addr.sin_addr.s_addr=inet_addr(input_addr);

    if((sockfd=socket(PF_INET,SOCK_STREAM,0))==-1){
        perror("create socket");
        return -1;
    }

    if (!strcmp(socketType,"server")){

        if (bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))==-1){
            perror("bind");
            return -1;
        }

        if (listen(sockfd,lisnum)==-1){
            perror("listen");
            return -1;
        }

        if((fd=accept(sockfd,(struct sockaddr *)&their_addr,&socklen))==-1){
            perror("accept");
            return -1;
        }

    }else if(!strcmp(socketType,"client")){

          if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) != 0)
        {
            perror("Connect ");
            exit(EXIT_FAILURE);
        }
    }

    return fd;
}


/*int hostDecide(){
    if () {
       code
    }
}*/

void syncTime(){
    while(1){
        /*if (dev.character) {/////////////////segment fault?
            dev.character=hostDecide();
        }*/

        /*switch(dev.msgState){
            case sync:
                if (dev.state==wait_send){
                    out(sync);///////////////////can it just one sigle line to operate, like function array
                }
        }*/
        if (dev.state==wait_send) {
            if(out(dev.msgState)!=-1){
                if (dev.msgState==2&&dev.character==host) {
                    dev.state=done;
                    break;
                }else if(dev.msgState<2){
                    dev.msgState++;////////////////////////undone
                    dev.state=wait_recive;
                }else{
                    break;
                }

            }else{
                perror("send");
            }
        }else if(dev.state==wait_recive&&dev.msgState!=done){
            dev.msg[dev.msgState]=receive(dev.msgState);
            if (dev.msg[dev.msgState]!=NULL){
                if (dev.msgState==3&&dev.character==slave) {
                    dev.state=done;
                    break;
                }else if(dev.msgState<3){
                    dev.msgState++;////////////////////////undone
                    dev.state=wait_send;
                }else{
                    break;
                }
            }else{
                perror("receive");
            }
        }else{
            break;
        }
    }
}



int main(int argc,char *argv[]){
    if(!strcmp(argv[1],"--help")){
        printf("help\n");
    }
    else if (argc < 2){
        printf("Please add IP argument:IP-address port socket-mode(server/client)\n");
        exit(1);
    }

    char *c="y";
    while(strcasecmp(c,"n")){

        if (strcasecmp(argv[3],"server")) {
            dev.character=host;//////////tempral host decide, server is host
            dev.fd=getSocket(argv[1],argv[2],argv[3]);
        }else{
            dev.character=slave;
            dev.fd=getSocket(argv[1],argv[2],"client");
        }

        if (dev.fd!=-1){
            printf("Connect success.\n");
            break;
        } else{
            printf("Connect fail.\n");
            printf("Try again?(y) Quit?(n):");
            scanf("%s",c);
        }
    }

    while(1){
        syncTime();
    }


}
