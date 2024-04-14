#ifndef RECEIVER_HANDLER_H
#define RECEIVER_HANDLER_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../message.h"
#include "../chat_node.h"

void* handle_receive(void *arg);
void* receive_from_server(void* arg);

#endif