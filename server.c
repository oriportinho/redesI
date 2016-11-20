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

void server(int port1, int port2) {
  int sock1_fd, sock2_fd, listen_fd;
  socklen_t cli1_len, cli2_len;

  struct sockaddr_in serv_addr, cli1_addr, cli2_addr;

  // listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  sock1_fd = socket(AF_INET, SOCK_STREAM, 0);
  sock2_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&cli1_addr, sizeof(cli1_addr));
  bzero(&cli2_addr, sizeof(cli2_addr));

  cli1_addr.sin_family = AF_INET;
  cli1_addr.sin_addr.s_addr = htons(INADDR_ANY);
  cli1_addr.sin_port = htons(port1);

  cli2_addr.sin_family = AF_INET;
  cli2_addr.sin_addr.s_addr = htons(INADDR_ANY);
  cli2_addr.sin_port = htons(port2);

  if(bind(sock1_fd, (struct sockaddr *) &cli1_addr, sizeof(cli1_addr)) < 0) {
    perror("ERRO on binding in socket 1.\n");
    exit(1);
  }

  if(bind(sock2_fd, (struct sockaddr *) &cli2_addr, sizeof(cli2_addr)) < 0) {
    perror("ERRO on binding in socket 2.\n");
    exit(1);
  }

  // listen(listen_fd, 5);
  listen(sock1_fd, 6);
  listen(sock2_fd, 7);

  cli1_len = sizeof(cli1_addr);
  cli2_len = sizeof(cli2_addr);

  printf("lkklfskf\n");
	while(true) {
    int pid = 0;//fork();
    int sock1_accept = accept(sock1_fd, (struct sockaddr *) &cli1_addr, &cli1_len);
    if(sock1_accept < 0) {
      perror("ERROR on accept in socket 1");
      exit(1);
    }

    int n;
    char buffer[256];
    bzero(buffer, 256);
    n = read(sock1_fd, buffer, 255);
    write(sock2_fd, buffer, 255);

    if(n < 0) {
      perror("ERRO reading from socket.\n");
      exit(1);
    }
printf("%d\n", pid);

printf("DSSD");


printf("Port %d say: %s\n", sock1_fd, buffer);
    //if(pid == 0) {
    //  exit(0);

    // }else{
    //   int sock2_accept = accept(sock2_fd, (struct sockaddr *) &cli2_addr, &cli2_len);
    //   if(sock2_accept < 0) {
    //     perror("ERROR on accept in socket 2");
    //     exit(1);
    //   }
    //
    //   int n;
    //   char buffer[256];
    //   bzero(buffer, 256);
    //   n = read(sock2_fd, buffer, 255);
    //
    //   if(n < 0) {
    //     perror("ERRO reading from socket.\n");
    //     exit(1);
    //   }
    //
    //   printf("Port %d say: %s\n", sock2_fd, buffer);
    //   // exit(0);

    // }
    /////////
  }
}


int main (int argc, char *argv[]) {
  int port1 = atoi(argv[1]);
  int port2 = atoi(argv[2]);

  server(port1, port2);

}
