#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define MAX_CLIENTS 2
#define MAX_BUFFER 1025

int main(int argc , char *argv[]) {
  int opt = 1;
  int master_socket, addrlen, new_socket, client_socket[MAX_CLIENTS];
  int activity, valread, sd, max_sd, port;
  struct sockaddr_in address;
  char buffer[MAX_BUFFER];
  char *message = "\n";

  if(argc >= 2) {
    port = atoi(argv[1]);
  }else {
    port = 20050;
  }
  fd_set readfds;

  // Initialise all client_socket[] to 0 so not checked
  for(int i = 0; i < MAX_CLIENTS; i++) {
    client_socket[i] = 0;
  }

  // Create a master socket
  if((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set master socket to allow multiple connections
  if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( port );

  // Bind the socket
  if(bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  printf("Listener on port %d \n", port);

  // Try to specify maximum of 3 pending connections for the master socket
  if(listen(master_socket, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // Accept the incoming connection
  addrlen = sizeof(address);
  puts("Waiting for connections ...");

  while(true) {
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

    //add child sockets to set
    for(int i = 0; i < MAX_CLIENTS ; i++) {
      sd = client_socket[i];

      if(sd > 0) {
        FD_SET( sd , &readfds);
      }
      if(sd > max_sd) {
        max_sd = sd;
      }
    }

    // Wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely
    activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

    if((activity < 0) && (errno!=EINTR)) {
      printf("select error");
    }

    // If something happened on the master socket , then its an incoming connection
    if(FD_ISSET(master_socket, &readfds)) {
      if((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      // Inform user of socket number - used in send and receive commands
      printf("New connection , socket fd is %d , ip is : %s , port : %d \n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

      // Send new connection greeting message
      if(send(new_socket, message, strlen(message), 0) != strlen(message)) {
        perror("send");
      }

      // Add new socket to array of sockets
      for(int i = 0; i < MAX_CLIENTS; i++) {
        if(client_socket[i] == 0) {
          client_socket[i] = new_socket;
          printf("Adding to list of sockets as %d\n" , i);
          break;
        }
      }
    }

    for(int i = 0; i < MAX_CLIENTS; i++) {
      sd = client_socket[i];

      if(FD_ISSET(sd, &readfds)) {

        // Check if it was for closing, and also read the incoming message
        if((valread = read(sd, buffer, 1025)) == 0) {
          // Somebody disconnected, get his details and print
          getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
          printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
          close(sd);
          client_socket[i] = 0;

        }else {
          buffer[valread] = '\0';
          printf("Client %d say: ", i);
          puts(buffer);

          // Send the mensagem for all clients
          for(int j = 0; j < MAX_CLIENTS; j++) {
            if(i != j) {
              send(client_socket[j] , buffer , strlen(buffer) , 0 );
            }else {
              send(sd , "\n" , 1 , 0 );
            }
          }
        }
      }
    }
  }

  return 0;
}
