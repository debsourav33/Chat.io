Steps:

1. Naviagate to project/ directory
2. Edit the info.properties, put the current host's ip, user name, port and server ip & server port
3. To run the server side application, first build using the make file make_server.mk
   $ make -f make_server.mk 
   Then run the executable
   $ ./build_server/chat_server 
4. To run the client side application, first build using the make file make_client.mk
   $ make -f make_client.mk 
   Then run the executable
   $ ./build_client/chat_client
5. Start sending (and receiving!) messages (join first using 'JOIN') command

