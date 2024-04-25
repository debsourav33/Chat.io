#ifndef MESSAGE_H
#define MESSAGE_H

#include "chat_node.h"
#include<string.h>



#define JOIN 1
#define JOINING 2
#define LEAVE 3
#define LEFT 4
#define SHUTDOWN 5
#define SHUTDOWN_ALL 6
#define NOTE 7
#define IS_CONNECTED 8 //special command that's sent from client to inquire server if it is connected

typedef char Note[64];

typedef struct message_struct{
    unsigned char type;
    ChatNode chat_node;
    Note note;
} Message;

//parses message type and note from the given input
Message parse_message(char* input);


#endif //MESSAGE_H
