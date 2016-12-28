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
