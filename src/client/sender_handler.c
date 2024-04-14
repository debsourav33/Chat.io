#include "sender_handler.h"


char* properties_file = "info.properties";

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

void* handle_sender(void* arg){

    ChatNode myself = parse_my_chatnode();
    char input[256];

    while(1){
        get_user_input(input);
        
        Message msg = parse_message(input);
        msg.chat_node = myself;
        printf("(type = %d) %s\n",msg.type,msg.note);

        send_to_server(msg);
    }
    
    //then parse the type and note data from the input using function in message.c 
    //Message msg = parse(input)

    //verify and send the message to the server
    //if(verify(msg, status))  send_to_server(msg);
}

void get_user_input(char* input){
    //use gets() or other scan function to get input from user
    puts("Type your message: ");
    fgets(input, 256, stdin);
}



void send_to_server(Message msg){
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
}