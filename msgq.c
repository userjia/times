#include "main.h"
struct device dev;
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

void msgqSend(int msgid,int type,struct delivery deli){
    struct msg_buf *msg_send;
    //memset(&msg_send,'\0',sizeof(struct msg_buf));
    msg_send=malloc(sizeof(struct msg_buf)+strlen(deli.op)+1);
    msg_send->type=type;
    //memcpy(&msg_send.msg,&deli,sizeof(deli));
    msg_send->msg=deli;
    int val=msgsnd(msgid,msg_send,strlen(deli.op)+1,0);//??
    if (val>0){
        printf("send msgq %d\n",type);
    }else{
        perror("msgq send");
    }
}

struct msg_buf msgqRecv(int msgid,int type){
    struct msg_buf msg_recv;
    //msg_recv->msg.op=malloc(BUFSIZE);
    //memset(msg_recv,'\0',sizeof(struct msg_buf));
    memset(&msg_recv,'\0',sizeof(msg_recv));
    msg_recv.type=type;
    printf("waiting for msgq %d\n",type);
    ssize_t rcv=msgrcv(msgid,&msg_recv,sizeof(struct msg_buf)+BUFSIZE+1,msg_recv.type,0);
    if (rcv>0){
        printf("get msgq %ld, size %ld\n",msg_recv.type,rcv);
    }else{
        perror("msg_buf receive");
    }
    //msgctl(msgid,IPC_RMID,0);
    return msg_recv;
}

void listenCommand(int msgid){
    struct msg_buf buf;
    struct delivery deli;
    char c[2];
    while(1){
        buf=msgqRecv(msgid,1);
        if (buf.msg.op!=NULL){
            printf("%s\n",buf.msg.op);
            if(strcmp(buf.msg.op,"get offset")){
                memcpy(deli.op,"offset",strlen("offset")+1);
                deli.offset=dev.offset;
                deli.delay=dev.delay;
                msgqSend(msgid,2,deli);
            }else if(strcmp(buf.msg.op,"quit sync")){
                memcpy(deli.op,"ready to quit",strlen("ready to quit"));
                msgqSend(msgid,2,deli);
                exit(0);
            }else if(strcmp(buf.msg.op,"sync now")){

            }
        } else{
            perror("msgq receive");
            printf("try again?");
            scanf("%s",c);
            if (strcasecmp(c,"y")==0){
                continue;
            } else{
                break;
            }
        }
    }
}

void sendCommand(int msgid,char *cmd){
    struct delivery deli;
    memcpy(deli.op,cmd,strlen(cmd));
    msgqSend(msgid,1,deli);

}

void listenSend(int msgid){
    struct msg_buf buf;
    while (1){
        buf=msgqRecv(msgid,2);
        if (buf.msg.op!=NULL){
            printf("%s\n",buf.msg.op);
            if (buf.msg.offset.tv_usec){
                printf("offset:%ld-%ld\n",buf.msg.delay.tv_sec,buf.msg.offset.tv_usec);
            }
            if (buf.msg.delay.tv_usec){
                printf("delay:%ld-%ld\n",buf.msg.delay.tv_sec,buf.msg.delay.tv_usec);
            }
            break;
        } else{
            continue;
        }
    }
}
