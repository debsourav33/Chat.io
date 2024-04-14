#include "client_handler.h"

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
#include <syslog.h>

#define SERVER_PORT 1357

void handle_clients()
{   
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    int yes = 1;

    
    // ----------------------------------------------------------
    // create unnamed network socket for server to listen on
    // ----------------------------------------------------------
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    // lose the pesky "Address already in use" error message
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // ----------------------------------------------------------
    // bind the socket
    // ----------------------------------------------------------
    server_address.sin_family      = AF_INET;           // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
    server_address.sin_port        = htons(SERVER_PORT);       // port to listen on
    
    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0) 
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    
    // ----------------------------------------------------------
    // listen on the socket
    // ----------------------------------------------------------
    if (listen(server_socket, 1) != 0)
    {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }
    
    // ----------------------------------------------------------
    // server loop
    // ----------------------------------------------------------
    while (1)
    {   
        int x = 15;
        printf("Square of %d is retrieved from chatnode to be %d\n",x,get_square(x));
        puts("Waiting for a client");
        // accept connection to client
        int client_socket = accept(server_socket, NULL, NULL);
        printf("\nServer with PID %d: accepted client\n", getpid());

        // create thread to handle the client's request
        // note that this is a naive approach, i.e. there are race conditions
        // for now this is okay, assuming low load
       pthread_t thread;
       int ret = pthread_create(&thread, NULL, talk_to_client, (void*) &client_socket);
       if(ret != 0){
        printf("Error creating pthread");
        exit(EXIT_FAILURE);
       }
       
       //automatically clean up resources when the thread exits - we don't need to join with it later
       pthread_detach(thread);
    }
    
}


/* ************************************************************************* */
/* handle client                                                             */
/* ************************************************************************* */

void* talk_to_client(void* arg) 
{
    int client_socket = *((int*)arg);   // the socket connected to the client

    // get the message from client
    Message msg;
    read(client_socket, &msg, sizeof(Message));

    char *name = msg.chat_node.name;
    Note note;
    strcpy(note, msg.note);

    switch (msg.type) {
        case JOIN:
            // send JOINING message to all other participants
            break;
        case LEAVE:
            // send LEAVING message to all other participants
        case SHUTDOWN:
            // send LEAVING message to all other participants
            break;
        case SHUTDOWN_ALL:
            // send SHUTDOWN message to all other participants
            break;
        case NOTE:
            
            printf("%s: %s\n",name,note);
            break;
       
    }


    pthread_exit(NULL);
}