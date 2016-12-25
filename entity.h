//
// Created by jp on 12/23/16.
//

#ifndef TIMES_ENTITY_H
#define TIMES_ENTITY_H
#define MASTER 0
#define SLAVE 1

struct message{
    enum messageState messageType;
    unsigned short messageLength;
    unsigned short sequenceID;//报文序列号
    signed char logMessageInterval;//报文发送间隔
};

enum deviceCharacter{
    host,
    slave,
};

enum messageState{
    sync,
    follow_up,
    delay_req,
    delay_resp,
};

enum deviceState{
    wait_send,
    wait_recive,
    done,
};

struct device{
    int fd;
    enum deviceCharacter character;
    enum deviceState state;
    enum messageState msgState;
    struct message *msg[4];
};

#endif //TIMES_ENTITY_H
