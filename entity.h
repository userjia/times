//
// Created by jp on 12/23/16.
//

#ifndef TIMES_ENTITY_H
#define TIMES_ENTITY_H

struct timeStamp{
    struct timeval tv;
    struct timezone tz;
};

struct delivery{
    struct timeval offset;
    struct timeval delay;
    char op[32];
};


struct msg_buf{
    long type;
    struct delivery msg;
};

struct message{
    int messageType;///why enum can't
    struct timeStamp stime;
    struct timeStamp rtime;
    struct timeStamp content;
};

struct connet{
    char *addr;
};

struct config{
    char title[20];
    char content[64];
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


struct device{
    int fd;
    struct sockaddr_in addr;
    struct sockaddr_in their_addr;
    enum deviceCharacter character;
    enum deviceState state;
    enum messageState msgState;
    struct message *msg[4];
    struct timeval offset;
    struct timeval delay;
    struct config configs[5];
};

#endif //TIMES_ENTITY_H
