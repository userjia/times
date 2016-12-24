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
void out(struct message msg){

}

struct message receive(){

}

char *getTime(){
    time_t t;
    struct tm *localTime;
    t= time(NULL);
    localTime=localtime(&t);
    return asctime(localTime);
}

struct message setMessage(struct message msg){

}


int getSeverSocket(char *addr,char *port){
    int sockfd,fd;
    int lisnum=1;//for listening client extend
    struct sockaddr_in my_addr,their_addr;
    socklen_t socklen;

    bzero(&my_addr, sizeof(my_addr));//TODO what is bzero,htons?
    my_addr.sin_family=PF_INET;
    my_addr.sin_port=htons(atoi(port));
    my_addr.sin_addr.s_addr=inet_addr(addr);
    
    if((sockfd=socket(PF_INET,SOCK_STREAM,0))==-1){
        perror("create socket");
        return -1;
    }

    if (bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))==-1){
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

    return fd;
}

int getClientSocket(char *addr,char *port){

}

int (* getSocket)(char *addr,char *port);

int hostDecide(){}

int main(int argc,char *argv[]){
    if(!strcmp(argv[1],"--help")){
        printf("help\n");
    }
    else if (argc < 2){
        printf("Please add IP argument:IP-address port socket-mode(server/client)\n");
        exit(1);
    }

    if (!strcmp(argv[3],"server")){
        getSocket=&getSeverSocket;
    }else if(!strcmp(argv[3],"client")){
        getSocket=&getClientSocket;
    }

    struct device dev;
    char *c="y";
    while(!strcasecmp(c,"y")){

        dev.fd=(*getSocket)(argv[1],argv[2]);
        if (dev.fd!=-1){
            printf("Connect success.\n");
            break;
        } else{
            printf("Connect fail.\n");
            printf("Try again?(y) Quit?(n):");
            scanf("%s",c);
        }
    }

    //printf("Please choose the mode(host[0]/slave[1]):");
    //scanf("%d",c);

    while(1){
        if (dev.fd==hostDecide()){
            switch(dev.msgState){
                case sync:
                    if (dev.state==wait_send){
                        out(sync);///////////////////can it just one sigle line to operate, like function array
                    }
            }
        }
        receive(dev.fd);/////////////////?
        break;
    }

}