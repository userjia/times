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
    msg_send=malloc(sizeof(struct msg_buf));
    bzero(msg_send,sizeof(*msg_send));
    msg_send->type=type;
    //memcpy(&msg_send.msg,&deli,sizeof(deli));
    msg_send->msg=deli;
    int val=msgsnd(msgid,msg_send,sizeof(struct msg_buf),0);
    if (val>0){
        printf("send msgq.\n type:%d\n op:%s",type,deli.op);
    }else{
        perror("msgq send");
    }
}

struct msg_buf *msgqRecv(int msgid,int type){
    struct msg_buf *msg_recv;
    msg_recv=malloc(sizeof(struct msg_buf));
    memset(msg_recv,'\0',sizeof(msg_recv));
    msg_recv->type=type;
    printf("waiting for msgq %d\n",type);
    ssize_t rcv=msgrcv(msgid,msg_recv,sizeof(struct msg_buf),msg_recv->type,0);
    if (rcv>0){
        printf("get msgq %ld, size %ld\n",msg_recv->type,rcv);
    }else{
        perror("msg_buf receive");
    }
    //msgctl(msgid,IPC_RMID,0);
    return msg_recv;
}

void listenCommand(int msgid){
    struct msg_buf *bufm;
    struct delivery deli;
    char c[2];
    char *result;
    char op[64];
    while(1){
        bufm=msgqRecv(msgid,1);
        if (bufm->msg.op!=NULL){
            strcpy(op,bufm->msg.op);
            printf("get command:%s\n",op);
            if(strcmp(op,"get offset")==0){
                result="offset";
                strcpy(deli.op,result);
                deli.offset=dev.offset;
                deli.delay=dev.delay;
                msgqSend(msgid,2,deli);
            }else if(strcmp(op,"quit sync")==0){
                result="ready to quit";
                bzero(&deli,sizeof(deli));
                memcpy(deli.op,result,strlen(result)+1);
                msgqSend(msgid,2,deli);
                exit(0);
            }else if(strcmp(op,"sync now")==0){

            }
        } else{
            perror("msgq receive\n");
            printf("try again?\n");
            scanf("%s",c);
            if (strcasecmp(c,"y")==0){
                continue;
            } else{
                break;
            }
        }
    }
}


void sendCommand(int msgid,char cmd[]){
    struct delivery deli;
    bzero(&deli,sizeof(deli));
    strcpy(deli.op,cmd);
    msgqSend(msgid,1,deli);
}

void listenSend(int msgid){
    struct msg_buf *buf;
    while (1){
        buf=msgqRecv(msgid,2);
        if (buf->msg.op!=NULL){
            printf("%s\n",buf->msg.op);
            if (buf->msg.offset.tv_usec!=0||buf->msg.offset.tv_sec!=0){
                printf("offset:%ld-%ld\n",buf->msg.delay.tv_sec,buf->msg.offset.tv_usec);
            }else{
                printf("offset is 0");
            }
            if (buf->msg.delay.tv_usec!=0||buf->msg.delay.tv_sec!=0){
                printf("delay:%ld-%ld\n",buf->msg.delay.tv_sec,buf->msg.delay.tv_usec);
            }else{
                printf("delay is 0");
            }
            break;
        } else{
            printf("op is null");
            continue;
        }
    }
}
