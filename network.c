#include "network.h"

int network(){
  {

    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    pthread_t thread;
    int thread_ret;
    int option=1;
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
      printf("Could not create socket\n");
    }
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 9999 );
    
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
      printf("bind failed!!!!!\n");
    } else {
      printf("bind succeeded\n");
      
      //Listen
      listen(socket_desc , 3);
      
      //Accept and incoming connection
      printf("Waiting for incoming connections...\n");
      c = sizeof(struct sockaddr_in);
      while (doorgaan==0){
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (new_socket<0){
	  printf("accept failed\n");
	} else{
	  printf("Connection accepted\n");
	  thread_ret=pthread_create(&thread, NULL, worker, (void *) &new_socket);
	  //worker(new_socket);
	}
      }
      printf("We need to stop\n");
    }
  }
  
  return 0;
}




