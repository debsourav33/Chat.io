## How to run:

- Naviagate to `project/` directory
- Edit the `info.properties`, put the
  + Current host's IP
  + Current host's port
  + Current host's name
  + Server IP
  + Server port
- To run the server side application, 
   * Build the server: `$ make -f make_server.mk`
   * Start the server: `$ ./build_server/chat_server` 
- To run the client side application, 
   * Build the client: `$ make -f make_client.mk` 
   * Start the client: `$ ./build_client/chat_client`
- Start sending (and receiving!) messages (make sure to join first using the `'JOIN'` command)

