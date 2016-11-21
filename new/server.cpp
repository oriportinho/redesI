#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

#define PORTA 8585
#define BYTE 1024

main(){
  char mensagem[BYTE], *loc;
  int pontarq, tbuf, skt, escolha;
  socklen_t tskt;
  struct sockaddr_in serv;

  system("clear");
  /**INICIALIZA ESTRUTURA SOCKETS*/
  skt = socket(AF_INET, SOCK_STREAM, 0);
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = INADDR_ANY;
  serv.sin_port = htons (PORTA);
  memset(&(serv.sin_zero),0x00,sizeof(serv.sin_zero));
  tskt = sizeof(struct sockaddr_in);

  printf("\n    ############### Server ###############\n\n");
  /**SOCKET INICIALIZA LISTENER PARA OUVIR PORTA*/
  bind(skt,(struct sockaddr *)&serv,sizeof(struct sockaddr));
  listen(skt,1);
  printf(">> Servidor esta escutando na porta %d\n\n",PORTA);

  /**RECEBE NOVAS CONEXÕES*/
  //O problema acho que está aqui, preciso receber mais uma conexão para o sistema continuar.
  skt = accept(skt,(struct sockaddr *)&serv,&tskt);
  printf(">> A Conexao com o endereco %s foi estabelecida\n\n",inet_ntoa(serv.sin_addr));


  /**ENVIA MENSAGEM PARA CLIENTE*/
  strcpy(mensagem,"Servidor diz: olá!!!");
  send(skt,mensagem,strlen(mensagem), 0);
  // sendto()
  // send()
  // sent
  /**RECEBE MENSAGEM DE CLIENTE*/
  tbuf = recv(skt, mensagem,BYTE, 0);
  mensagem[tbuf]=0x00;
  printf(">: %s\n",mensagem);


  /**LOOP DE COMUNICAÇÃO ENTRE CLIENTE E SERVIDOR*/
  do{
    ///recebe
    tbuf = recv(skt,mensagem,BYTE,0);
    mensagem[tbuf]=0x00;
    printf(">: Cliente diz: %s\n",mensagem);

    ///envia
    printf("> ");
    gets(mensagem);
    send(skt, mensagem, strlen(mensagem), 0);


  }while(strcmp(mensagem,"/x") != 0); ///COMUNICAÇÃO SE ENCERRA QUANDO USUARIO ENVIAR MSG= /X

  close(skt);
  printf(">> A Conexao com o host %s foi encerrada!!!\n\n",inet_ntoa(serv.sin_addr));
  exit(0);
}
