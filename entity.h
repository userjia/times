//
// Created by jp on 12/23/16.
//

#ifndef TIMES_ENTITY_H
#define TIMES_ENTITY_H
#define MASTER 0
#define SLAVE 1

/*struct timeStamp{
    struct timeval tv;
    struct timezone tz;
};

struct timeStamp{
    struct timespec tv;
};*/

struct msg_buf{
    long type;
    struct delivery *msg;
};

struct message{
    int messageType;///why enum can't
    //unsigned short messageLength;
    //unsigned short sequenceID;//报文序列号
    //signed char logMessageInterval;//报文发送间隔

    //struct timeStamp stime;
    //struct timeStamp rtime;
    //struct timeStamp content;
    //struct message *content;

    struct timespec stime;
    struct timespec rtime;
    struct timespec content;
};

struct connet{
    char *addr;
};

enum deviceCharacter{
    host,
    slave,
};

enum messageState{
    sync_time,
    follow_up,
    delay_req,
    delay_resp,
};

enum deviceState{
    wait_recive,
    wait_send,
    done,
};

struct delivery{
    char *op;
    struct timespec offset;
    struct timespec delay;
};

struct device{
    int fd;
    struct sockaddr_in addr;
    struct sockaddr_in their_addr;
    enum deviceCharacter character;
    enum deviceState state;
    enum messageState msgState;
    struct message *msg[4];
    struct timespec offset;
    struct timespec delay;
};

struct config{
    char title[20];
    char content[64];
    struct config *next;
};

#endif //TIMES_ENTITY_H
