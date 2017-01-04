//
// Created by jp on 12/29/16.
//
#include "main.h"

struct device dev;
void getConfig(){
    FILE *fp;
    char buf[64];
    fp=fopen("./config","r");
    if (fp==NULL){
        perror("open config");
        exit(-4);
    }
    int i=0;
    while(!feof(fp)) {
        fgets(buf,sizeof(buf),fp);
        if (strcmp(buf,"\n")){
            if (buf[0]=='#'){
                strcpy(dev.configs[i].title,buf);
            } else{
                strcpy(dev.configs[i].content,buf);
                i++;
            }
        }
    }

}
void configSocket(){
    getConfig();
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(dev.configs[0].content);
    addr.sin_port=htons(atoi(dev.configs[1].content));
    dev.addr=addr;
    if (strcasecmp(dev.configs[2].content,"server\n")==0) {
        dev.character=host;//////////tempral host decide, server is host
        dev.state=wait_send;
        dev.waitRecv=0;
    }else{
        dev.character=slave;
        dev.state=wait_recive;
        dev.their_addr.sin_family=AF_INET;
        dev.their_addr.sin_addr.s_addr=inet_addr(dev.configs[3].content);
        dev.their_addr.sin_port=htons(atoi(dev.configs[4].content));
        dev.waitRecv=1;
    }
}
