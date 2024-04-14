#ifndef SENDER_HANDLER_H
#define SENDER_HANDLER_H

#include "../message.h"
#include "../chat_node.h"
#include "../properties.h"

#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>

void get_user_input(char* input);
    
bool verify(Message msg, int current_status);

void send_to_server(Message msg);
void* handle_sender(void* arg);

#endif