#include "client_main.h"


int main(){
    pthread_t thread[2];

    //start sender thread    
    int ret = pthread_create(&thread[0], NULL, handle_sender, NULL);
    if(ret != 0){
        printf("Error creating pthread");
        exit(EXIT_FAILURE);
    }

    //start receiver thread
    ret = pthread_create(&thread[1], NULL, handle_receive, (void*) NULL);
    if(ret != 0){
        printf("Error creating pthread");
        exit(EXIT_FAILURE);
    }

    pthread_join(thread[1], NULL);


    return 0;
}