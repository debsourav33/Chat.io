#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "../message.h"
#include "../chat_node.h"

void handle_clients();
void* talk_to_client(void* arg);

#endif