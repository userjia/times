//
// Created by jp on 12/30/16.
//
#include "main.h"

void main(int argc,char *argv[]){
    char cmd[64];
    int msgid=getMsgid("/home/jp/ClionProjects/times-l/times/msgqtest");

    while (strcmp(cmd,"quit")!=0){
        printf("please input the command:\n");
        fgets(cmd,sizeof(cmd),stdin);
        sendCommand(msgid,cmd);
        listenSend(msgid);
    }
}
