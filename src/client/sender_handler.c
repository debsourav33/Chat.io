#include "sender_handler.h"
#include "../text_color.h"

char* properties_file = "info.properties";

#define JOINED_STATE 1
#define NOT_JOINED_STATE 2

int current_state;

ChatNode parse_my_chatnode(){
    ChatNode node;

    Properties* properties;
    properties = property_read_properties(properties_file);
    
    char* name = property_get_property(properties, "MY_NAME");
    char* ip = property_get_property(properties, "MY_IP");
    char* port = property_get_property(properties, "MY_PORT");

    strcpy(node.name, name);
    strcpy(node.ip, ip);
    node.port = atoi(port);

    return node;
}

void parse_server_info(char* server_ip, int *server_port){
    Properties* properties;
    properties = property_read_properties(properties_file);

    char* ip = property_get_property(properties, "SERVER_ADDR");
    char* port = property_get_property(properties, "SERVER_PORT");

    strcpy(server_ip, ip);
    *server_port = atoi(port);
}

ChatNode myself;

//why do we need to ping server for initial state?
//because a client may just exit the app after joining, so server holds its state as joined, 
//however, upon relaunch, client has no idea to know by itself if it's joined, so it consults server
void get_current_state_from_server(){
    Message msg;
    msg.type = IS_CONNECTED;
    msg.chat_node = myself;

    int state = send_to_server(msg);

    current_state = state;
    if(current_state == JOINED_STATE) puts("Currently Joined"); else puts("Currently Not Joined");
}

void* handle_sender(void* arg){

    myself = parse_my_chatnode();
    char input[256];

    get_current_state_from_server();

    while(1){

        //get input from user
        get_user_input(input);

        //then parse the type and note data from the input using function in message.c     
        Message msg = parse_message(input);
        msg.chat_node = myself;
        //printf("(type = %d) %s\n",msg.type,msg.note);

        //verify and send the message to the server
        if(verify(msg))
            send_to_server(msg);
    }
}


//verify if a certain message type is eligible to be sent based on sender's current state (joined or not joined)
bool verify(Message msg){
    switch (msg.type)
    {   
        //can only join if not already joined
        case JOIN:
            if(current_state == JOINED_STATE){
                printf("You have already joined\n");
                return false;
            }

            current_state = JOINED_STATE;
            return true;
        
        //can only send note, leave or shutdown if already joined
        case NOTE:
            if(current_state != JOINED_STATE){
                printf("You have to join first!\n");
                return false;
            }
            //no state change for NOTE
            return true;

        case LEAVE:
        case SHUTDOWN:
        case SHUTDOWN_ALL:
            if(current_state != JOINED_STATE){
                printf("You have to join first!\n");
                return false;
            }

            current_state = NOT_JOINED_STATE;
            return true;

        default:
            printf("Unknown %d\n",msg.type);
            return false;
    }
}

void get_user_input(char* input){
    //use gets() or other scan function to get input from user
    //printf("%s%s:%s ",NOTE_COLOR,myself.name,RESET_COLOR);
    fgets(input, 256, stdin);
    input[strlen(input)-1] = '\0'; //fgets get the newline too -> trim it
}



int send_to_server(Message msg){
    //send the msg to the server

    int client_socket;                  // client side socket
    struct sockaddr_in client_address;  // client socket naming struct
    char c;
    
    //parse server info from properties file
    char server_addr[32];
    int port;
    parse_server_info(server_addr, &port);


    // create an unnamed socket, and then name it
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    // create addr struct
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(server_addr);
    client_address.sin_port = htons(port);
    
    // connect to server socket
    if (connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) {
        perror("Error connecting to server!\n");
        exit(EXIT_FAILURE);
    }
    
    //send the message object to the server
    write(client_socket, &msg, sizeof(Message));

    //for is_connected message, server sends back reply
    if(msg.type == IS_CONNECTED){
        Message reply;
        read(client_socket, &reply, sizeof(Message));

        printf("Server's state reply: %d\n",reply.type);
        if(reply.type == JOIN)  return JOINED_STATE;
        return NOT_JOINED_STATE;
    }


    //special case - if it's a shutdown message, then we don't wait for server response, we sleep 2 second and terminate
    if(msg.type == SHUTDOWN){
        sleep(2);
        exit(EXIT_SUCCESS);
    }

    return 0;
}