#include "client_handler.h"
#include "chat_participants.h"

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

ChatNodeList nodes;

void* broadcast(Message msg);

pthread_mutex_t client_lock; 

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
        puts("Waiting for a client");
        // accept connection to client
        int client_socket = accept(server_socket, NULL, NULL);
        printf("\nServer with PID %d: accepted client\n", getpid());

        // create thread to handle the client's request
        // note that this is a naive approach, i.e. there are race conditions
        // for now this is okay, assuming low load
       pthread_t thread;
       int ret = pthread_create(&thread, NULL, talk_to_client, (void*) &client_socket);
       pthread_mutex_lock(&client_lock); //handle race condition for client_socket read

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
    pthread_mutex_unlock(&client_lock); //unlock client-socket read mutex

    // get the message from client
    Message msg;
    read(client_socket, &msg, sizeof(Message));

    char *name = msg.chat_node.name;
    Note note;
    strcpy(note, msg.note);


    printf("%s: (type = %d) %s %s\n",name,msg.type,note,msg.chat_node.ip);

    ChatNode node;

    switch (msg.type) {
        case JOIN:
            node = get_participant_from_list(&nodes, msg);
            
            //don't add existing participants again
            if(node.port != -1){
                printf("%s already joined\n",msg.chat_node.name);
                break;
            }  

            printf("%s has joined\n",msg.chat_node.name);
            
            add_participant_to_list(&nodes, msg.chat_node);
            msg.type = JOINING; //this person is joining the chat
            broadcast(msg);
            break;
        case LEAVE:
        case SHUTDOWN:
            node = get_participant_from_list(&nodes, msg);
            
            //if user not joined, they can't leave/shutdown
            if(node.port == -1){
                printf("%s has not joined\n",msg.chat_node.name);
                break;
            }  

            printf("%s has left\n",msg.chat_node.name);

            
            msg.type = LEFT;
            broadcast(msg);

            remove_participant(&nodes, node);

            break;
        case SHUTDOWN_ALL:
            // send SHUTDOWN message to all other participants
            msg.type = SHUTDOWN;

            puts("Shut down all");
            broadcast(msg);
            remove_all(&nodes); //remove all participants

            break;
        case NOTE:
            
            node = get_participant_from_list(&nodes, msg);

            //if not joined, discard the note
            if(node.port == -1){
                printf("%s has not joined\n",msg.chat_node.name);
                break;
            }  

            printf("Already joined node: {%s}\n",node.name);
            broadcast(msg);
            
            break;
        case IS_CONNECTED:
            //client wants to know if it has already joined
            node = get_participant_from_list(&nodes, msg);
            
            Message reply;
            if(node.port != -1)  reply.type = JOIN; //if connected, reply message type is JOIN
            else  reply.type = LEAVE; //else reply message type is LEAVE

            write(client_socket, &reply, sizeof(Message));
            break;
    }


    pthread_exit(NULL);
}

void send_message(ChatNode node, Message msg){
    //ChatNode node  = *((ChatNode*) arg);
    int client_socket;                  // client side socket
    struct sockaddr_in client_address;  // client socket naming struct
    char c;

    // create an unnamed socket, and then name it
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    // create addr struct
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(node.ip);
    client_address.sin_port = htons(node.port);
    
    // connect to server socket of the client
    if (connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) {
        perror("Error connecting to server!\n");
        exit(EXIT_FAILURE);
    }
    
    //send the message object to the server
    write(client_socket, &msg, sizeof(Message));
}

//send to all participants
void* broadcast(Message msg){
    ChatNodeListElement* curr = nodes.head;
    while(curr != NULL){
        
        //don't send the message to the owner himself
        //if(strcmp(curr->node.ip, msg.chat_node.ip) == 0)  continue;
        
        send_message(curr->node, msg);
        curr = curr->next;
    }
}

