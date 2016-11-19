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
  printf("--------------------TESTE--------------------\n");
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

	while(true) {
    printf("t\n");
    sock1_fd = accept(listen_fd, (struct sockaddr *) &cli1_addr, &cli1_len);
    printf("t\n");
    sock2_fd = accept(listen_fd, (struct sockaddr *) &cli2_addr, &cli2_len);
    printf("t\n");

		if(sock1_fd < 0) {
      perror("ERROR on accept in socket 1");
      exit(1);
    }
    if(sock2_fd < 0) {
      perror("ERROR on accept in socket 2");
      exit(1);
    }

    int pid = fork();
    printf("fork\n");
    ////////
    if(pid == 0) {
      printf("f1\n");
      int n;
      char buffer[256];
      bzero(buffer, 256);
      printf("TESTE01\n");
      n = read(sock1_fd, buffer, 255);
      printf("TESTE02\n");

      if(n < 0) {
        perror("ERRO reading from socket.\n");
        exit(1);
      }

      printf("Port %d say: %s\n", sock1_fd, buffer);
      exit(0);

    }else{
      int n;
      char buffer[256];
      bzero(buffer, 256);
      printf("TESTE11\n");
      n = read(sock2_fd, buffer, 255);
      printf("TESTE12\n");

      if(n < 0) {
        perror("ERRO reading from socket.\n");
        exit(1);
      }

      printf("Port %d say: %s\n", sock2_fd, buffer);
      exit(0);

    }
    /////////
  }
}


int main (int argc, char *argv[]) {
  printf("TESTE01\n");

  int port1 = atoi(argv[1]);
  int port2 = atoi(argv[2]);

  printf("TESTE01\n");
  server(port1, port2);
  printf("TESTE01\n");

}
