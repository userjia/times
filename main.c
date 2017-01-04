//
// Created by jp on 12/21/16.
//


#include "main.h"

struct device dev;
pthread_t circleThread;
int main(int argc,char *argv[]){

    /*//daemon
    int pid;
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGHUP,SIG_IGN);
    if(pid=fork()) {
        exit(EXIT_SUCCESS);
    } else if(pid<0){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    setsid();
    if(pid=fork()) {
        exit(EXIT_SUCCESS);
    } else if(pid<0){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < NOFILE; ++i) {
        close(i);
    }
    open("/dev/null",O_RDONLY);
    open("/dev/null",O_RDWR);
    chdir("/temp");
    umask(0);
    signal(SIGCHLD,SIG_IGN);
    *////

    configSocket();
    getSocket();
    dev.serverTime=1;
    if (dev.character==slave){
        pthread_create(&circleThread,NULL,(void *)circleSync,NULL);
        int msgid=getMsgid("/home/jp/ClionProjects/times-l/times/msgqtest");
        dev.msgqid=msgid;
        listenCommand(msgid);
    } else{

        circleSync();

    }
}
