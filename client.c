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

  connect(port,(struct sockaddr *)&servaddr,sizeof(servaddr));

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

void server(int port, int port_client) {
  // char buffer[100];
  int newsockfd, listen_fd;
  socklen_t clilen;

  struct sockaddr_in servaddr, cli_addr;
  int pid;

  listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if(bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
    perror("ERRO on binding.\n");
    exit(1);
  }

  listen(listen_fd, 5);
  clilen = sizeof(cli_addr);

	while(true) {
    newsockfd = accept(listen_fd, (struct sockaddr *) &cli_addr, &clilen);

		if(newsockfd < 0) {
			perror("ERROR on accept");
			exit(1);
		}

		/* Create child process */
		pid = fork();

		if(pid < 0) {
			perror("ERROR on fork");
			exit(1);
		}

		if(pid == 0) {
      int n;
      char bufferb[256];

      bzero(bufferb, 256);
      n = read(newsockfd, bufferb, 255);

      if(n < 0) {
        perror("ERRO reading from socket.\n");
        exit(1);
      }

			printf("Port %d say: %s\n", port_client, bufferb);

			exit(0);

		}else {
			close(newsockfd);
		}

	}
}

int main(int argc, char *argv[]) {

  int port_server = atoi(argv[1]);
  int port_client;

  printf("What is a destiny port?\n");
  scanf("%d\n", &port_client);

  int pid = fork();

  if(pid == 0) {
    server(port_server, port_client);
  }else {
    client(port_client);
  }

}
