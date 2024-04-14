#include "receiver_handler.h"


#define SERVER_PORT 7531

void* handle_receive(void *arg)
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
        
        // accept connection to client
        int client_socket = accept(server_socket, NULL, NULL);
        printf("\nServer with PID %d: accepted client\n", getpid());

        // create thread to handle the client's request
        // note that this is a naive approach, i.e. there are race conditions
        // for now this is okay, assuming low load
       pthread_t thread;
       int ret = pthread_create(&thread, NULL, receive_from_server, (void*) &client_socket);
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

void* receive_from_server(void* arg) 
{
    int client_socket = *((int*)arg);   // the socket connected to the client

    // get the message from client
    Message msg;
    read(client_socket, &msg, sizeof(Message));

    switch (msg.type) {
        case JOINING:
            // print joining message for the participant
            break;
        case LEFT:
            // print leaving message for the participant
        case SHUTDOWN_ALL:
            // print a termination message and terminate the client
            break;
        case NOTE:
            // get the note and sender's name and print the note + sender's names 
            break;
    }


    pthread_exit(NULL);
}