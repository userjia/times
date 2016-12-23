//
// Created by jp on 12/21/16.
//

#ifndef TIMES_MESSAGE_H
#define TIMES_MESSAGE_H

struct message{
    unsigned char messageType;
    unsigned short messageLength;
    unsigned short sequenceID;//报文序列号
    signed char logMessageInterval;//报文发送间隔
};

#endif //TIMES_MESSAGE_H
