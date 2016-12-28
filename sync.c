//
// Created by jp on 12/28/16.
//
#include "main.h"
struct device dev;
char buf[sizeof(struct message)];
ssize_t out(enum messageState ms){
    struct message msg;
    msg.messageType=ms;///msg is not full
    gettimeofday(&msg.stime.tv,&msg.stime.tz);//send time

    memset(buf,'\0',sizeof(buf));
    memcpy(buf,&msg,sizeof(msg));
    ssize_t len=sendto(dev.fd,buf,strlen(buf),0,(struct sockaddr *) &dev.their_addr, sizeof(dev.their_addr));// TODO struct point
    if (len==-1) {
        perror("send");
    }
    return len;
}

struct message *receive(enum messageState ms){
    struct message *msg;
    msg=(struct message *)malloc(sizeof(struct message));
    socklen_t addr_len=sizeof(dev.their_addr);

    //char* buf[sizeof(msg)];
    memset(buf,'\0',sizeof(buf));
    ssize_t len=recvfrom(dev.fd,buf,sizeof(buf)-1,0,(struct sockaddr *) &dev.their_addr,&addr_len);
    memset(msg,'\0',sizeof(msg));
    memcpy(msg,buf,sizeof(buf));

    gettimeofday(&msg->rtime.tv,&msg->rtime.tz);//recv time
    if (len!=-1) {
        printf("the %d received\n",msg->messageType);
        return msg;
    }else{
        //perror("receive");
        return NULL;
    }
}

/*char *getTime(){
    time_t t;
    struct tm *localTime;
    t= time(NULL);
    localTime=localtime(&t);
    return asctime(localTime);
}*/


/*int hostDecide(){
    if () {
       code
    }
}*/

int syncTime(){
    char in[2];
    ssize_t len;
    while(1){
        if (dev.state==wait_send) {
            printf("send %d?(y/n)\n",dev.msgState);
            scanf("%s",in);
            if (strcasecmp(in,"y")==0){
                len=out(dev.msgState);
                if(len==-1){
                    printf("send error, continue %d?(y/n)\n",dev.msgState);
                    scanf("%s",in);
                    if (strcasecmp(in,"y")==0){
                        continue;//error,try again
                    } else{
                        return -1;
                    }
                } else{
                    printf("resend?(y/n)\n");
                    scanf("%s",in);
                    if (strcasecmp(in,"y")==0){
                        continue;//error,try again
                    }
                }
            }else{
                printf("cancel send\n");
            }
        }else{
            printf("waiting for %d\n",dev.msgState);
            dev.msg[dev.msgState]=receive(dev.msgState);
            if(dev.msg[dev.msgState]==NULL){
                perror("receive");
                printf("continue receive?(y/n)");
                scanf("%s",in);
                if (strcasecmp(in,"y")==0){
                    continue;
                }else{
                    return -1;
                }
            }
        }
        if (dev.msgState==delay_resp&&((dev.state+dev.character)==1)) {// state and character different value
            dev.state=done;
            break;
        }else if(dev.msgState!=sync){
            //dev.state=(dev.state/(-1))+1;/////0 to 1 or 1 to 0
            if (dev.state==0){
                dev.state=wait_send;
            }else{
                dev.state=wait_recive;
            }
        }
        dev.msgState++;//TODO segment fault may be here ,fuck
    }
    return 0;

}


struct timeval add(struct timeval a,struct timeval b){
    struct timeval time={a.tv_sec-b.tv_sec,a.tv_usec-b.tv_usec};
    return time;
}

struct timeval sub(struct timeval a,struct timeval b){
    struct timeval time={a.tv_sec+b.tv_sec,a.tv_usec+b.tv_usec};
    return time;
}

struct timeval devide(struct timeval a){
    struct timeval time={a.tv_sec/2,a.tv_usec/2};
    return time;
}


void calculate(){

    struct timeval t1=dev.msg[0]->stime.tv;
    struct timeval t2=dev.msg[0]->rtime.tv;
    struct timeval t3=dev.msg[2]->stime.tv;
    struct timeval t4=dev.msg[2]->rtime.tv;

    dev.delay=devide(add(sub(t2,t1),sub(t4,t3)));
    dev.offset=devide(sub(add(t1,t2),add(t4,t3)));
}

void *circleSync(){
    char in[2];
    while(1){
        if(syncTime()==-1){
            perror("sync");
            break;
        }
        calculate();
        printf("sync again?(y/n)\n");
        if(strcasecmp(in,"y")==0){
            continue;
        } else{
            break;
        }
    }

}