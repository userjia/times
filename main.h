//
// Created by jp on 12/28/16.
//

#ifndef TIMES_MAIN_H_H
#define TIMES_MAIN_H_H

#include <time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <pthread.h>
#include "entity.h"
#include <unistd.h>
#include <bits/signum.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/param.h>
#include <error.h>
#include <bits/sigthread.h>

#define BUFSIZE 64
extern struct device dev;
extern char buf[sizeof(struct message)];
extern void *circleSync();
extern pthread_t circleThread;

extern void listenCommand(int a);
extern void sendCommand(int a,char *cmd);
extern void listenSend(int a);
extern int getMsgid(char *a);
extern void msgqSend(int msgid,int type,struct delivery deli);
extern int getSocket();
extern int confirmSock(int character);
extern void configSocket();
#endif //TIMES_MAIN_H_H
