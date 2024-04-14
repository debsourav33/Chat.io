
#include "message.h"
#include "chat_node.h"

//ChatNode get_participant_from_list(Message msg);
//void add_participant_to_list(ChatNode node);

ChatNodeList list;

ChatNode get_participant_from_list(Message msg){
    ChatNodeListElement* curr = list.head;
    while(curr != NULL){
        if(strcmp(curr->node.ip, msg.chat_node.ip) == 0)  return curr->node;
        curr = curr -> next;
    }

    ChatNode node;
    return node;
}

void add_participant_to_list(ChatNode node){
    ChatNodeListElement* element;
    element->node = node;

    list.tail->next = element;
    list.tail = list.tail->next;
}

