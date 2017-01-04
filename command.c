//
// Created by jp on 12/30/16.
//
#include "main.h"

void main(int argc,char *argv[]){
    char cmd[64];
    int msgid=getMsgid("/home/jp/ClionProjects/times-l/times/msgqtest");//TODO absolute path, what if daemon?

    while (1){
        printf("please input the command:\n");
        //memset(cmd,'\0',sizeof(cmd));
        fgets(cmd,sizeof(cmd),stdin);
        cmd[strlen(cmd)-1]='\0';
        if(strcasecmp(cmd,"quit")==0){
            break;
        }
        //scanf("%s",&cmd);
        sendCommand(msgid,cmd);
        listenSend(msgid);
    }
    exit(0);
}
