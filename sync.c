//
// Created by jp on 12/28/16.
//
#include "main.h"
struct device dev;
char buf[sizeof(struct message)];
ssize_t out(enum messageState ms){
    struct message *msg;
    msg=(struct message *)malloc(sizeof(struct message));
    msg->messageType=ms;///msg is not full

    if(dev.character==host&&dev.msgState==follow_up){
        msg->content=dev.msg[dev.msgState-1]->stime;
    }
    if(dev.msgState==delay_resp){
        msg->content=dev.msg[dev.msgState-1]->rtime;
    }
    memset(buf,'\0',sizeof(buf));
    ssize_t len=sendto(dev.fd,msg,sizeof(*msg),0,(struct sockaddr *) &dev.their_addr, sizeof(dev.their_addr));
    if (len==-1) {
        perror("send");
    }
    if(dev.msgState==sync_time||dev.msgState==delay_req){
        gettimeofday(&(msg->stime.tv),&(msg->stime.tz));//send time
        dev.msg[dev.msgState]=msg;
    }
    return len;
}


struct message *receive(){
    struct message *msg;
    msg=(struct message *)malloc(sizeof(struct message));
    socklen_t addr_len=sizeof(dev.their_addr);

    memset(buf,'\0',sizeof(buf));
    ssize_t len=recvfrom(dev.fd,msg,sizeof(*msg)-1,0,(struct sockaddr *) &dev.their_addr,&addr_len);
    if (dev.msgState==sync_time||dev.msgState==delay_req){
        gettimeofday(&(msg->rtime.tv),&(msg->rtime.tz));//recv time
    }
    if (dev.msgState==follow_up){
        dev.msg[dev.msgState-1]->stime=msg->content;
    }
    if(dev.msgState==delay_resp){
        dev.msg[dev.msgState-1]->rtime=msg->content;
    }

    if (len!=-1) {
        printf("the %d received\n",msg->messageType);
        return msg;
    }else{
        return NULL;
    }
}

int syncTime(){
    char in[2];
    ssize_t len;
    while (dev.state != done) {
        if (dev.state == wait_send) {
            printf("send %d\n", dev.msgState);
            len = out(dev.msgState);
            if (len == -1) {
                printf("send error, continue %d?(y/n)\n", dev.msgState);
                scanf("%s", in);
                if (strcasecmp(in, "y") == 0) {
                    continue;//error,try again
                } else {
                    return -1;
                }
            }
        } else {
            printf("waiting for %d\n", dev.msgState);
            dev.msg[dev.msgState] = receive();
            if (dev.msg[dev.msgState] == NULL) {
                perror("receive");
                printf("continue receive?(y/n)");
                scanf("%s", in);
                if (strcasecmp(in, "y") == 0) {
                    continue;
                } else {
                    return -1;
                }
            }
        }
        if (dev.msgState == delay_resp &&
            ((dev.state + dev.character) == 1)) {// state and character different value
            dev.state = done;
            break;
        } else if (dev.msgState != sync_time) {
            if (dev.state == 0) {
                dev.state = wait_send;
            } else {
                dev.state = wait_recive;
            }
        }
        dev.msgState = dev.msgState + 1;
    }
    return 0;
}


struct timeval sub(struct timeval a,struct timeval b){
    struct timeval time;
    time.tv_sec=a.tv_sec-b.tv_sec;
    time.tv_usec=a.tv_usec-b.tv_usec;
    return time;
}

struct timeval add(struct timeval a,struct timeval b){
    struct timeval time;
    time.tv_sec=a.tv_sec+b.tv_sec;
    time.tv_usec=a.tv_usec+b.tv_usec;
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
    printf("%d:%d\n%d:%d\n%d:%d\n%d:%d\n",t1.tv_sec,t1.tv_usec,t2.tv_sec,t2.tv_usec,t3.tv_sec,t3.tv_usec,t4.tv_sec,t4.tv_usec);
    dev.delay=devide(add(sub(t2,t1),sub(t4,t3)));
    dev.offset=devide(sub(sub(t2,t1),sub(t4,t3)));
}

void *circleSync(){
    char in[2]="y";
    while (strcasecmp(in, "y") == 0) {
        if(confirmSock(dev.character)==0) {
            if (syncTime() == -1) {
                perror("sync_time");
                break;
            }
            if (dev.character == slave) {
                calculate();
                printf("delay is %d-%d\n", dev.delay.tv_sec, dev.delay.tv_usec);
                printf("offset is %d-%d\n", dev.offset.tv_sec, dev.offset.tv_usec);
                dev.state=wait_recive;
            } else{
                dev.state=wait_send;
            }
            for (int i = 0; i < 4; ++i) {
                dev.msg[i] = NULL;
            }
            dev.msgState = sync_time;
            printf("sync_time again?(y/n)\n");
            scanf("%s", in);
        }
    }
    return 0;
}