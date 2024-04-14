#ifndef CHAT_P_H
#define CHAT_P

#include <stdlib.h>
#include "message.h"
#include "chat_node.h"

ChatNode get_participant_from_list(ChatNodeList* list, Message msg);
void add_participant_to_list(ChatNodeList* list, ChatNode node);
void remove_participant(ChatNodeList* list, ChatNode node);
void remove_all(ChatNodeList* list);

#endif