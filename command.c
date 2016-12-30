//
// Created by jp on 12/30/16.
//
#include "main.h"



void main(int argc,char *argv[]){
    char cmd[64];
    fgets(cmd,sizeof(cmd),stdin);
    while (strcmp(cmd,"quit")!=0){
        sendCommand(argv[0],cmd);
        listenSend(argv[0]);
    }
}
