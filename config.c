//
// Created by jp on 12/29/16.
//
#include "main.h"
#include "entity.h"
/*
void getConfig(){
    FILE *fp;
    fp=fopen("./config","r");

    struct config *head=(struct config *)malloc(sizeof(struct config));
    struct config *p=head;
    while(!feof(fp)){
        struct config *buf=(struct config *)malloc(sizeof(struct config));
        fgets(buf->title,sizeof(buf->title),fp);
        fgets(buf->content,sizeof(buf->content),fp);
        p->next=buf;
        p=buf;
    }
}

void showConfig(struct config *head){
    struct config *p=head;
    while(p->next!=NULL){
        p=p->next;
        printf("%s:%s\n",p->title,p->content);
    }
}

void setSocket(){

    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(argv[1]);
    addr.sin_port=htons(atoi(argv[2]));
    dev.addr=addr;

    if (strcasecmp(argv[3],"server")==0) {
        dev.character=host;//////////tempral host decide, server is host
        dev.state=wait_send;
    }else{
        dev.character=slave;
        dev.state=wait_recive;
        dev.their_addr.sin_family=AF_INET;
        dev.their_addr.sin_addr.s_addr=inet_addr(argv[4]);
        dev.their_addr.sin_port=htons(atoi(argv[5]));

    }
}
*/