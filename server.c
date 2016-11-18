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

void server(int port) {
  // char buffer[100];
  int sockfd, newsockfd, listen_fd;
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
			close(sockfd);
      int n;
      char bufferb[256];

      bzero(bufferb, 256);
      n = read(newsockfd, bufferb, 255);

      if(n < 0) {
        perror("ERRO reading from socket.\n");
        exit(1);
      }

			printf("Port %d say: %s\n", bufferb);

			exit(0);

		}else {
			close(newsockfd);
		}

	}
}
