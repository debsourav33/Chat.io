#include "message.h"
#include<stdio.h>

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

//utility function to turn all chars to capital letters
void capitalize(char* str){
    for(int i=0;i<strlen(str);i++){
        if(str[i]>='a' && str[i]<='z')  str[i] -= 32;
    }
}

Message parse_message(char* str){
    Message msg;

    char prefix[256];
    get_prefix(str, prefix); //gets the str until a space or new line found
    capitalize(prefix); //so that 'Join' and 'JOIN' both can be accepted

    //printf("p = %s\n",prefix);
    //printf("s = %s\n",suffix);

    //suffix will be the note portion, unless the entire message is note
    if(strcmp(prefix, JOIN_STR) == 0){
        msg.type = JOIN;
    }
    else if(strcmp(prefix, LEAVE_STR) == 0){
        msg.type = LEAVE;
    }
    else if(strcmp(prefix, SHUTDOWN_STR) == 0){
        msg.type = SHUTDOWN;
    }
    else if(strcmp(prefix, SHUTDOWN_ALL_STR) == 0){
        msg.type = SHUTDOWN_ALL;
    }
    else{ //else the entire message is a note
        msg.type = NOTE;        
        strcpy(msg.note, str); //copy entire message
    }

    return msg;
}