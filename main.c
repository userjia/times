//
// Created by jp on 12/21/16.
//

#include <time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "entity.h"


struct device dev;
char *buf[sizeof(struct message)];

long int out(enum messageState ms){
    struct message msg;
    msg.messageType=ms;///msg is not full
    gettimeofday(&msg.stime.tv,&msg.stime.tz);//send time

    memset(buf,0,sizeof(buf));
    memcpy(buf,&msg,sizeof(msg));
    long int len=sendto(dev.fd,buf,sizeof(msg),0,(struct sockaddr *) &dev.addr, sizeof(dev.addr));// TODO struct point
    if (len==-1) {
        perror("send");
    }

    return len;
}

struct message *receive(enum messageState ms){
    struct message *msg;
    msg=(struct message *)malloc(sizeof(struct message));
    socklen_t addr_len=sizeof(dev.their_addr);

    //char* buf[sizeof(msg)];
    memset(buf,0,sizeof(buf));
    long int len=recvfrom(dev.fd,buf,sizeof(buf),0,(struct sockaddr *) &dev.their_addr,&addr_len);
    memset(msg,0,sizeof(msg));
    memcpy(msg,buf,sizeof(buf));
    gettimeofday(&msg->rtime.tv,&msg->rtime.tz);//recv time
    if (len!=-1) {
        return msg;
    }else{
        perror("receive");
        return NULL;
    }
}

/*char *getTime(){
    time_t t;
    struct tm *localTime;
    t= time(NULL);
    localTime=localtime(&t);
    return asctime(localTime);
}*/

int getSocket(char *input_addr,char *port,char *socketType){
    int sockfd,fd;
    int lisnum=1;//for listening client extend
    struct sockaddr_in addr,their_addr;
    socklen_t socklen;

    bzero(&addr, sizeof(addr));//TODO what is bzero,htons?
    addr.sin_family=PF_INET;
    addr.sin_port=htons(atoi(port));
    addr.sin_addr.s_addr=inet_addr(input_addr);

    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("create socket");
        return -1;
    }
    dev.fd=sockfd;
    if (!strcmp(socketType,"server")){
        if (bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))==-1){
            perror("bind");
            return -1;
        }

    }else if(!strcmp(socketType,"client")){
        dev.addr=addr;
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
        if (dev.state==wait_send) {
            if(!out(dev.msgState)){
                continue;//error,try again
            }
        }else{
            dev.msg[dev.msgState]=receive(dev.msgState);
            if(dev.msg[dev.msgState]==NULL){
                continue;
            }
        }
        if (dev.msgState==delay_resp&&((dev.state+dev.character)==1)) {// state and character different value
            dev.state=done;
            break;
        }else if(dev.msgState!=sync){
            dev.state=(dev.state/-1)+1;/////0 to 1 or 1 to 0
        }
        dev.state++;
    }

}

struct timeval add(struct timeval a,struct timeval b){
    return {a.tv_sec-b.tv_sec,a.tv_usec-b.tv_usec};
}

struct timeval sub(struct timeval a,struct timeval b){
    return {a.tv_sec+b.tv_sec,a.tv_usec+b.tv_usec};
}

struct timeval devide(struct timeval a){
    return {a.tv_sec/2,a.tv_usec/2};
}

void calculate(){

    struct timeval t1=dev.msg[0]->stime.tv;
    struct timeval t2=dev.msg[0]->rtime.tv;
    struct timeval t3=dev.msg[2]->stime.tv;
    struct timeval t4=dev.msg[2]->rtime.tv;

    dev.delay=devide(add(sub(t2,t1),sub(t4,t3)));
    dev.offset=devide(sub(add(t1,t2),add(t4,t3)));
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
            dev.state=wait_send;
            dev.fd=getSocket(argv[1],argv[2],argv[3]);
        }else{
            dev.character=slave;
            dev.state=wait_recive;
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
        calculate();
    }


}
