#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void client(int port) {
  int sockfd;
  char sendline[100];
  char recvline[100];
  struct sockaddr_in servaddr;

  bzero(&servaddr, sizeof servaddr);

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));

  connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));


  while(true) {
    bzero(sendline, 100);
    bzero(recvline, 100);

    fgets(sendline, 100, stdin);
    strtok(sendline, "\n");
    if(sendline[0] == '0') {
      exit(0);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0) {
      perror("ERRO opening socket.\n");
    }

    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
      perror("ERRO connecting.\n");
    }

    if(write(sockfd, sendline, strlen(sendline)+1) < 0) {
      perror("ERRO reading from socket.\n");
    }

    close(sockfd);

  }

}


int main(int argc,char **argv) {

}
