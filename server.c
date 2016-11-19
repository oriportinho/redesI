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
  printf("TESTE");
  int sock1_fd, sock2_fd, listen_fd;
  socklen_t cli1_len, cli2_len;

  struct sockaddr_in serv_addr, cli1_addr, cli2_addr;

  listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  if(bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERRO on binding.\n");
    exit(1);
  }

  listen(listen_fd, 5);
  cli1_len = sizeof(cli1_addr);
  cli2_len = sizeof(cli2_addr);

	while(true) {
    sock1_fd = accept(listen_fd, (struct sockaddr *) &cli1_addr, &cli1_len);
    sock2_fd = accept(listen_fd, (struct sockaddr *) &cli2_addr, &cli2_len);

		if(sock1_fd < 0 || sock2_fd < 0) {
			perror("ERROR on accept");
			exit(1);
		}
    int pid = fork();

    if(pid == 0) {
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
  }
}


int main (int argc, char *argv[]) {
  printf("TESTE01\n");

  int port_server = atoi(argv[1]);

  printf("TESTE01\n");
  server(port_server);
  printf("TESTE01\n");

}
