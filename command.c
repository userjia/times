//
// Created by jp on 12/30/16.
//
#include "main.h"



void main(int argc,char *argv[]){
    char cmd[64];
    fgets(cmd,sizeof(cmd),stdin);
    while (strcmp(cmd,"quit")!=0){
        int msgid=getMsgid("../msgqtest");
        sendCommand(msgid,cmd);
        listenSend(msgid);
    }
}
