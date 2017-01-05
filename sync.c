//
// Created by jp on 12/28/16.
//
#include <asm/errno.h>
#include "main.h"
struct device dev;
ssize_t out(){
    struct message *msg;
    msg=(struct message *)malloc(sizeof(struct message));
    memset(msg,0,sizeof(msg));
    msg->messageType=dev.msgState;
    if(dev.character==host&&dev.msgState==follow_up){
        msg->content=dev.msg[dev.msgState-1]->stime;
    }
    if(dev.msgState==delay_resp){
        msg->content=dev.msg[dev.msgState-1]->rtime;
    }
    ssize_t len=sendto(dev.fd,msg,sizeof(*msg),0,(struct sockaddr *) &dev.their_addr, sizeof(dev.their_addr));
    if (len==-1) {
        perror("send");
    }
    if(dev.msgState==sync_time||dev.msgState==delay_req){
        clock_gettime(CLOCK_MONOTONIC_RAW,&(msg->stime.tv));//////////////////////TODO send timestanp
        dev.msg[dev.msgState]=msg;
    }
    return len;
}


struct message *receive(){
    struct message *msg;
    msg=(struct message *)malloc(sizeof(struct message));
    memset(msg,0,sizeof(msg));
    socklen_t addr_len=sizeof(dev.their_addr);
    memset(msg,'\0',sizeof(msg));
    ssize_t len=recvfrom(dev.fd,msg,sizeof(*msg)-1,0,(struct sockaddr *) &dev.their_addr,&addr_len);
    if (dev.msgState==sync_time||dev.msgState==delay_req){
        clock_gettime(CLOCK_MONOTONIC_RAW,&(msg->rtime.tv));//////////////////////TODO recv timestanp
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
    dev.needResend=0;
    memset(&dev.delay,0,sizeof(dev.delay));
    memset(&dev.offset,0,sizeof(dev.offset));
    while (dev.state != done) {
        if (dev.state == wait_send) {
            printf("send %d\n", dev.msgState);
            len = out();
            if (len == -1) {
                return -1;
            }
        } else if(dev.state==wait_recive) {
            printf("waiting for %d\n", dev.msgState);
            dev.msg[dev.msgState] = receive();
            if (dev.msg[dev.msgState] == NULL) {
                perror("receive");
                return -1;
            }else if(dev.msg[dev.msgState]->messageType!=dev.msgState){
                dev.needResend=1;
                return -2;
            }
        }
        if (dev.msgState == delay_resp && ((dev.state + dev.character) == 1)) {// state and character different value
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


struct timespec sub(struct timespec a,struct timespec b){
    struct timespec time;
    time.tv_sec=a.tv_sec-b.tv_sec;
    time.tv_nsec=a.tv_nsec-b.tv_nsec;
    return time;
}

struct timespec add(struct timespec a,struct timespec b){
    struct timespec time;
    time.tv_sec=a.tv_sec+b.tv_sec;
    time.tv_nsec=a.tv_nsec+b.tv_nsec;
    return time;
}

struct timespec devide(struct timespec a){
    struct timespec time={a.tv_sec/2,a.tv_nsec/2};
    return time;
}


void calculate(){
    struct timespec t1=dev.msg[0]->stime.tv;
    struct timespec t2=dev.msg[0]->rtime.tv;
    struct timespec t3=dev.msg[2]->stime.tv;
    struct timespec t4=dev.msg[2]->rtime.tv;
    printf("%d:%d\n%d:%d\n%d:%d\n%d:%d\n",t1.tv_sec,t1.tv_nsec,t2.tv_sec,t2.tv_nsec,t3.tv_sec,t3.tv_nsec,t4.tv_sec,t4.tv_nsec);
    dev.delay=devide(add(sub(t2,t1),sub(t4,t3)));
    dev.offset=devide(sub(sub(t2,t1),sub(t4,t3)));
}

void sig(int s){
    if(s==SIGUSR1){
        printf("start sync immediately\n");
        dev.immediate=1;
    } else{
        printf("unknown signal %d",s);
    }
}

void *circleSync(){
    //char in[2]="y";
    //while (strcasecmp(in, "y") == 0) {
    struct timespec t={0,0};
    struct timespec rem;
    int confirm=-1;
    if (dev.character==slave){
        if (signal(SIGUSR1,sig)==SIG_ERR){//msgq 中断注册
            printf("catch signal fail");
        }
    }
    while (1) {
        confirm=confirmSock(dev.character);//与同步的另一台设备握手
        dev.serverTime++;
        if(confirm==0) {
            if (syncTime() == -1) {//同步时间，主要是获取消息及时间戳
                perror("sync_time");
                break;
            }

            if (dev.character == slave) {
                calculate();//计算offset、delay
                if (dev.immediate==1){//立即要求下发送msgq
                    struct delivery deli;
                    deli.offset=dev.offset;
                    deli.delay=dev.delay;
                    strcpy(deli.op,"immediate offset");
                    msgqSend(dev.msgqid,2,deli);
                }

                printf("delay is %d-%d\n", dev.delay.tv_sec, dev.delay.tv_nsec);//输出同步信息
                printf("offset is %d-%d\n", dev.offset.tv_sec, dev.offset.tv_nsec);
                printf("sync time is: %d\n\n\n\n\n\n\n",dev.serverTime-1);
                dev.state=wait_recive;

                if (clock_gettime(CLOCK_REALTIME,&t)==-1){//休眠一段时间
                    perror("clock gettime");
                }
                t.tv_sec+=3;
                int s=clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&t,NULL);
                if(s!=0){
                    if(s==EINTR){
                        printf("sleep interrupted");
                    } else{
                        perror("clock nanosleep");
                    }
                }
            } else{
                dev.state=wait_send;
                printf("sync time is: %d\n\n\n\n\n\n\n",dev.serverTime-1);
            }
            for (int i = 0; i < 4; ++i) {
                if(dev.msg[i]!=NULL){
                    free(dev.msg[i]);
                }
                dev.msg[i] = NULL;
            }
            dev.msgState = sync_time;
            confirm=1;

            //printf("sync_time again?(y/n)\n");
            //scanf("%s", in);
        }
    }
    return 0;
}
