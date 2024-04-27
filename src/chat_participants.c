#include "chat_participants.h"
#include<pthread.h>

pthread_mutex_t list_mutex =  PTHREAD_MUTEX_INITIALIZER;;

ChatNode get_participant_from_list(ChatNodeList* list, Message msg){
    ChatNodeListElement* curr = list->head;

    //traverse and find
    while(curr != NULL){
        if(strcmp(curr->node.ip, msg.chat_node.ip) == 0)  return curr->node;
        curr = curr -> next;
    }

    //not found
    ChatNode node;
    node.port = -1;
    return node;
}

void add_participant_to_list(ChatNodeList* list, ChatNode node){
    // Lock the mutex before modifying the list
    pthread_mutex_lock(&list_mutex);

    ChatNodeListElement *element = (ChatNodeListElement *)malloc(sizeof(ChatNodeListElement));

    element->node = node;
    element->next = NULL;

    //if list empty, this node becomes head = tail
    if(list->head == NULL){
        list->head = element;
        list->tail = element;
    }
    else{
        //otherwise append to next of tail
        list->tail->next = element;
        list->tail = list->tail->next;
    }

    //unlock
    pthread_mutex_unlock(&list_mutex);
  
}

//node must be present in list
void remove_participant(ChatNodeList* list, ChatNode node){
    // Lock the mutex before modifying the list
    pthread_mutex_lock(&list_mutex);
  

    //node = head case
    if(strcmp(list->head->node.ip, node.ip) == 0) {
        
        if(list->head == list->tail){
            list->head = list->tail = NULL;
        }
        else{
            list->head = list->head -> next;
        }

        //must unlock mutex before every return
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    //node != head case
    ChatNodeListElement* curr = list->head;
    while(curr->next != NULL){
        if(strcmp(curr->next->node.ip, node.ip) == 0){ //the next node is to remove!
            //tail remove case
            if(curr->next == list->tail){
                list->tail = curr;
                list->tail->next = NULL;
            }
            else{ //not tail case
                curr->next = curr->next->next;
            }

            //must unlock mutex before every return
            pthread_mutex_unlock(&list_mutex);
            return;
        }
        curr = curr -> next;
    }
     
    //unlock
    pthread_mutex_unlock(&list_mutex);
    
}

void remove_all(ChatNodeList* list){
    // Lock the mutex before modifying the list
    pthread_mutex_lock(&list_mutex);

    list->head = list->tail = NULL;

    //unlock
    pthread_mutex_unlock(&list_mutex);
}

void print_participants(ChatNodeList* list){
    ChatNodeListElement* curr = list->head;
    while(curr != NULL){
       printf("%s (%s)",curr->node.name,curr->node.ip);
       curr = curr -> next;
    }
    puts("");
}