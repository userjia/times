//
// Created by jp on 12/21/16.
//
#include "main.h"

int getMsgid(char *a){
    key_t key;
    int msgid;

    if((key=ftok(a,100))==-1){//A??
        perror("ftok");
        //
    }
    if((msgid=msgget(key,0600|IPC_CREAT))==-1){//0600?
        perror("msgget");
    }

    return msgid;

}

void msgqSend(int msgid,struct delivery deli){

    struct msginfo buf;
    struct msg_buf msg_send;

    memset(&msg_send,'\0',sizeof(struct msg_buf));

    msg_send.type=1;

    memcpy(msg_send.msg,&deli,sizeof(deli));

    int val=msgsnd(msgid,(void *)&msg_send,sizeof(msg_send.msg),0);//??

}

struct msg_buf *msgqRecv(int msgid){

    struct msg_buf *msg_recv;
    msg_recv=malloc(sizeof(struct msg_buf));
    memset(msg_recv,'\0',sizeof(struct msg_buf));

    msg_recv->type=1;
    ssize_t rcv=msgrcv(msgid,(void *)&msg_recv,sizeof(msg_recv->msg),msg_recv->type,0);

    msgctl(msgid,IPC_RMID,0);

    return msg_recv;
}






void listenCommand(char *a){
    struct msg_buf *buf;
    int msgid=getMsgid(a);
    while(1){
        buf=msgqRecv(msgid);
        if (!strcmp(buf->msg->op,"get different time")){
            struct delivery deli={"",dev.offset,dev.delay};
            msgqSend(msgid,deli);
        }
    }
}

void sendCommand(char *a){
    struct msg_buf *buf;
    char cmd[INPUTBUF];
    int msgid=getMsgid(a);
    while(strcasecmp(cmd,"quit this")){
        fgets(cmd,sizeof(cmd),stdin);
        struct delivery deli={cmd,dev.offset,dev.delay};
        msgqSend(msgid,deli);
    }
}
int getSocket(char *input_addr,char *port,char *socketType){
    int sockfd;
    int fd=0;
    int lisnum=1;//for listening client extend
    struct sockaddr_in addr,their_addr;
    socklen_t socklen=sizeof(struct sockaddr);

    bzero(&addr, sizeof(addr));//TODO what is bzero,htons?
    addr.sin_family=PF_INET;
    addr.sin_port=htons(atoi(port));
    addr.sin_addr.s_addr=inet_addr(input_addr);

    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("create socket");
        return -1;
    }

    if (strcmp(socketType,"server")==0){
        if (bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))==-1){
            perror("bind");
            return -1;
        }
        if (listen(sockfd,lisnum)==-1){
            perror("listen");
            return -1;
        }
        printf("waiting for accept...\n");
        if ((fd = accept(sockfd, (struct sockaddr *) &their_addr, &socklen)) == -1) {
            perror("accept");
            return -1;
        } else{
            printf("accept client.\n");
        }

    }else if(strcmp(socketType,"client")==0){
        dev.addr=addr;

        if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) != 0)
        {
            perror("Connect ");
            return -1;
        }
        fd=sockfd;
    }

    return fd;
}

struct device dev;
char buf[sizeof(struct message)];
int main(int argc,char *argv[]){

    if(!strcmp(argv[1],"--help")){
        printf("help\n");
    }
    else if (argc < 2){
        printf("Please add IP argument:IP-address port socket-mode(server/client)\n");
        exit(1);
    }

    char c[2];
    printf("begin?\n");
    scanf("%s",c);
    while(strcasecmp(c,"y")==0){

        if (strcasecmp(argv[3],"server")==0) {
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
            if(strcasecmp(c,"n")){
                exit(-2);
            }
        }
    }

    pthread_t circleThread,commandThread;
    pthread_create(&circleThread,NULL,(void *)circleSync,NULL);
    while(1){

    }
    //pthread_create(&commandThread,NULL,(void *)listenCommand,argv[0]);
    //sendCommand(argv[0]);
}
