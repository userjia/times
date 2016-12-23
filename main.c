//
// Created by jp on 12/21/16.
//

#include <time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "message.h"
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


int getSeverSocket(char *addr,char *port,int lisnum){
    int sockfd,fd;
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


int main(int argc,char *argv[]){
    int lisnum=1;//for listening client extend
    if (argc < 2){
        printf("Please add IP argument: IP-address Port Mode(host/slave)\n");
        exit(1);
    }

    extern int fd=getSeverSocket(argv[1],argv[2],lisnum);
    if (fd!=-1){
        printf("Connect success.\n");
    }
    while(1){
        if (strcmp(argv[3],"host")){
            
        }
        receive(fd);
        break;
    }

}