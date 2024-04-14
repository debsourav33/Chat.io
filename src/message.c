#include "message.h"
#include<stdio.h>


#define JOIN 1
#define JOINING 2
#define LEAVE 3
#define LEFT 4
#define SHUTDOWN 5
#define SHUTDOWN_ALL 6
#define NOTE 7

const char *JOIN_STR = "JOIN";
const char *LEAVE_STR = "LEAVE";
const char *SHUTDOWN_STR = "SHUTDOWN";
const char *SHUTDOWN_ALL_STR = "SHUTDOWN_ALL";
const char *NOTE_STR = "NOTE";

void get_prefix(char* str, char* s){
    
    for(int i=0;i<strlen(str);i++){
        if(str[i]==' '){
            s[i] = '\0';
            return;
        }
        s[i] = str[i];
    }
    
    s[strlen(str)] = '\0';

}

Message parse_message(char* str){
    Message msg;

    char prefix[256];
    get_prefix(str, prefix); //gets the str until a space or new line found
    char *suffix = strchr(str, ' ') + 1; //returns the pointer to the first occurane of ' '

    //if(strlen(prefix))  printf("%s-%s\n",prefix,suffix);

    //suffix will be the note portion, unless the entire message is note
    if(strcmp(prefix, JOIN_STR) == 0){
        msg.type = JOIN;
        
        strcpy(msg.note, suffix);
    }
    else if(strcmp(prefix, LEAVE_STR) == 0){
        msg.type = LEAVE;
        
        strcpy(msg.note, suffix);
    }
    else if(strcmp(prefix, SHUTDOWN_STR) == 0){
        msg.type = SHUTDOWN;
        
        strcpy(msg.note, suffix);
    }
    else if(strcmp(prefix, SHUTDOWN_ALL_STR) == 0){
        msg.type = SHUTDOWN_ALL;
        
        strcpy(msg.note, suffix);
    }
    else{ //else the entire message is a note
        msg.type = NOTE;        
        strcpy(msg.note, str); //copy entire message
    }

    return msg;
}