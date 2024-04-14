
#include "chat_participants.h"


//ChatNode get_participant_from_list(Message msg);
//void add_participant_to_list(ChatNode node);


ChatNode get_participant_from_list(ChatNodeList* list, Message msg){
    ChatNodeListElement* curr = list->head;
    while(curr != NULL){
        if(strcmp(curr->node.ip, msg.chat_node.ip) == 0)  return curr->node;
        curr = curr -> next;
    }

    ChatNode node;
    node.port = -1;
    return node;
}

void add_participant_to_list(ChatNodeList* list, ChatNode node){
    ChatNodeListElement *element = (ChatNodeListElement *)malloc(sizeof(ChatNodeListElement));

    element->node = node;
    element->next = NULL;

    if(list->tail == NULL){
        list->head = element;
        list->tail = element;
    }
    else{
        list->tail->next = element;
    }

    list->tail = list->tail->next;
}

//node must be present in list
void remove_participant(ChatNodeList* list, ChatNode node){
    //head case
    if(strcmp(list->head->node.ip, node.ip) == 0) {
        
        if(list->head == list->tail){
            list->head = list->tail = NULL;
        }
        else{
            list->head = list->head -> next;
        }
        return;
    }

    //not head case
    ChatNodeListElement* curr = list->head;
    while(curr->next != NULL){
        if(strcmp(curr->next->node.ip, node.ip) == 0){
            curr->next = curr->next->next;
            return;
        }
        curr = curr -> next;
    }
}

void remove_all(ChatNodeList* list){
    list->head = list->tail = NULL;
}

