#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define MAX_BUFFER 1024
#define CIPHER_KEY 3

main(int argc , char *argv[]) {
  char buffer[MAX_BUFFER];
  int tbuf, skt, port;
  struct sockaddr_in serv;

  // Define port
  if(argc >= 2) {
    port = atoi(argv[1]);
  }else {
    port = 20050;
  }
  system("clear");

  // Inicialization socket
  skt = socket(AF_INET, SOCK_STREAM, 0);
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = inet_addr(SERVER_IP);
  serv.sin_port = htons (port);
  memset(&(serv.sin_zero), 0x00, sizeof (serv.sin_zero));

  // Comunication with server
  while(connect (skt, (struct sockaddr *)&serv, sizeof (struct sockaddr)) != 0) {
    int i=0;
    char dot[12] = "";
    // Waiting connections
    for(i = 0; i < 10; i++) {
      system("clear");
      printf("\n\nSearch for server.");
      printf("\nPlease wait %s\n\n", dot);
      strcat(dot,".");
      sleep(1);
    }
    strcpy(dot, "");
  }
  printf(">> Server connection: %s \n", SERVER_IP);
  printf(">> in port: %d \n", port);
  printf(">> To exit --> exit();\n\n");

  // Receive mensagem from server
  tbuf = recv(skt, buffer, strlen(buffer), 0);
  buffer[tbuf] = 0x00;
  printf("%s\n",buffer);

  // Send mensagem from server
  strcpy(buffer, "\n");
  send(skt, buffer, strlen(buffer), 0);

  // Main loop, comunications between client and server
   while(strcmp(buffer,"exit();") != 0) {
    // Try to implement a fork to parallel send and receive
    int pid = fork();

    if(pid == 0) {
      // Send a buffer
      printf(">: ");
      scanf("%s", buffer);
      send(skt, buffer, MAX_BUFFER, 0);
      exit(1);

    }else {

      // Receive a buffer
      tbuf = recv(skt, buffer, MAX_BUFFER, 0);
      buffer[tbuf] = 0x00;
      if (strlen(buffer) > 0) {
        if (buffer[0] != '\n') {

          // Application Cesar Chipher only in a letters
          for (size_t i = 0; i < MAX_BUFFER; i++) {
            if (buffer[i] >= 'a' && buffer[i] <= 'z') {
              buffer[i] = 'a' + ((buffer[i] + CIPHER_KEY - 'a') % 26);
            } else if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
              buffer[i] = 'A' + ((buffer[i] + CIPHER_KEY - 'A') % 26);
            }
          }

          // Print the answer in the client terminal
          printf("Client say: %s\n", buffer);
        }
      }
    }
  }

  // End connection
  close(skt);
  exit(0);
  return 0;
}
