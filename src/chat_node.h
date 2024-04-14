

//include guard so that in no source chat_node.h is imported twice
#ifndef CHAT_NODE_H
#define CHAT_NODE_H

typedef struct chat_node{
    char name[16];
    char ip[16];
    int port;
} ChatNode; 

typedef struct chat_node_list_element{
    ChatNode node;
    struct chat_node_list_element* next;
} ChatNodeListElement; 

typedef struct chat_node_list{
    ChatNodeListElement* head;
    ChatNodeListElement* tail;
} ChatNodeList; 


int get_square(int x);

#endif // CHAT_NODE_H