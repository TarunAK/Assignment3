#ifndef MSGQ_H
#include "db.h"
#define MSGQ_H
#define SID 5678
#define CID 8765
#define SIZE 12
typedef struct msg_buffer { 
    long msg_type; 
    data msg;
} message;
typedef struct receive {
    long msg_type;
    char s[SIZE];
} inout;
#endif