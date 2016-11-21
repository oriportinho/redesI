#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>


#define SERVER_IP "127.0.0.1"
#define BYTE 1024
#define PORTA 8888//8585
#define TITULO "\n    ############### BeM VinDo ###############\n\n"

main() {
  char mensagem[BYTE], *loc;
  int tbuf, skt, escolha;
  struct sockaddr_in serv;
  system("clear");

  // INICIALIZA ESTRUTURA SOCKETS
  skt = socket(AF_INET, SOCK_STREAM, 0);
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = inet_addr(SERVER_IP);
  serv.sin_port = htons (PORTA);
  memset (&(serv.sin_zero), 0x00, sizeof (serv.sin_zero));

  // INICIA COMUNICAÇÃO COM SERVIDOR
  while(connect (skt, (struct sockaddr *)&serv, sizeof (struct sockaddr)) != 0){
    int i=0;
    char dot[12] = "";
    for(i=0; i<4;i++){
      system("clear");
      printf(TITULO);
      printf("\n\nProcurando servidor.");
      printf("\nAguarde %s\n\n", dot);
      strcat(dot,".");
      sleep(1);
    }
    strcpy(dot, "");
  }
  printf(">> A Conexao com o Servidor %s foi estabelecida na porta %d \n\n",SERVER_IP,PORTA);
  printf(">> Envie /x pra sair \n\n");


  /**RECEBE MENSAGEM DO SERVIDOR*/
  tbuf = recv (skt, mensagem, BYTE, 0);
  mensagem[tbuf] = 0x00;
  printf (">: %s\n",mensagem);

  /**ENVIA MENSAGEM PARA O SERVIDOR*/
  strcpy(mensagem, "estabelecida coneccao.");
  send(skt, mensagem, strlen(mensagem), 0 );


  /**LOOP DE COMUNICAÇÃO ENTRE CLIENTE E SERVIDOR*/
  do{
    ///envia
    printf("> ");
    gets(mensagem);
    send(skt, mensagem, strlen(mensagem), 0);

    ///recebe
    tbuf = recv (skt, mensagem, BYTE, 0);
    mensagem[tbuf] = 0x00;
    printf (">: Servidor diz: %s\n",mensagem);

  }while(strcmp(mensagem,"/x")!= 0);    ///COMUNICAÇÃO SE ENCERRA QUANDO USUARIO DIGITAR /X


  /**FINALIZA CONEXÃO*/
  close(skt);
  printf (">>A conexao com o servidor foi finalizada!!!\n\n");
  exit(0);
}
