#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// #include <arpa/inet.h>
// #include <netinet/in.h>

#define SERVER_PORT  5432
// #define MAX_PENDING  5
#define MAX_LINE     256

int main(int argc, char * argv[]) {
  struct hostent *hp;
  struct sockaddr_in sin;
  char *host;
  char buf[MAX_LINE];
  int s;
  socklen_t len;

  if(argc == 2) {
    host = argv[1];
  }else {
    fprintf(stderr, "usage simplex-talk host\n");
    exit(1);
  }

  // traduz nome do host para endereco de IP do host
  hp = gethostbyname(host);
  if(hp) {
    fprintf(stderr, "simplex-talk: unknowm host: %s\n", host);
    exit(1);
  }

  // monta estrutura de dados do endereco
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  // bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
  sin.sin_port = htons(SERVER_PORT);

  // abertura abertura
  if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("simplex-talk: socket");
    exit(1);
  }
  if(connect(s, (struct sockaddr *)&sin , sizeof(sin)) < 0) {
    perror("simplex-talk: connect");
    close(s);
    exit(1);
  }

  // laco principal: obtem e envia linhas de texto
  while(fgets(buf, sizeof(buf), stdin)) {
    buf[MAX_LINE - 1] = '\0';
    len = strlen(buf) + 1;
    send(s, buf, len, 0);
  }

}
