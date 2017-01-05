//
// Created by jp on 12/21/16.
//


#include "main.h"

struct device dev;//device全局变量，存储tsocket设置，同步报文等信息
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
    *////守护进程模式，因项目处于开发阶段，开启后不便于获得输出

    configSocket();
    getSocket();//初始化socket

    dev.serverTime=1;//同步次数计数变量
    if (dev.character==slave){//character记录程序运行模式为主时钟还是为从时钟
        pthread_create(&circleThread,NULL,(void *)circleSync,NULL);//创建线程不断同步
        int msgid=getMsgid("/home/jp/ClionProjects/times-l/times/msgqtest");//监听message quewn;
        dev.msgqid=msgid;
        listenCommand(msgid);
    } else{
        circleSync();//服务端不需要接收msg quen，直接循环同步；
    }
}
