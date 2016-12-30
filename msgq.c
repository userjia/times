#include "main.h"

int getMsgid(char *a){
    key_t key;
    int msgid;

    if((key=ftok(a,100))==-1){//A??
        perror("ftok");
    }
    if((msgid=msgget(key,0600|IPC_CREAT))==-1){//0600?
        perror("msgget");
    }

    return msgid;
}

void msgqSend(int msgid,struct delivery deli){
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
    struct delivery deli;
    while(1){
        buf=msgqRecv(msgid);
       if(strcmp(buf->msg->op,"get offset")){
           deli.op="offset";
           deli.offset=dev.offset;
           deli.delay=dev.delay;
           msgqSend(msgid,deli);
       }else if(strcmp(buf->msg->op,"quit sync")){
           deli.op="ready to quit";
           msgqSend(msgid,deli);
           exit(0);
       }else if(strcmp(buf->msg->op,"sync now")){

       }
    }
}

void sendCommand(char *a,char *cmd){
    int msgid=getMsgid(a);
    fgets(cmd,sizeof(cmd),stdin);
    struct delivery deli;
    deli.op=cmd;
    msgqSend(msgid,deli);
}

void listenSend(char *a){
    struct msg_buf *buf;
    int msgid=getMsgid(a);
    buf=msgqRecv(msgid);
    printf("%s\n",buf->msg->op);
    if (buf->msg->offset.tv_usec){
        printf("offset:%ld-%ld\n",buf->msg->delay.tv_sec,buf->msg->offset.tv_usec);
    }
    if (buf->msg->delay.tv_usec){
        printf("delay:%ld-%ld\n",buf->msg->delay.tv_sec,buf->msg->delay.tv_usec);
    }
}
