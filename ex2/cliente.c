#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "net_api.h"

#define MAX_CLIENTS 30

int main(int argc, char **argv)
{
  char error[MAXLINE + 1];
  struct sockaddr_in servaddr;

  // verifica se foi passado o ip e a porta para o servidor de processamento se conectar
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
    perror(error);
    exit(1);
  }

  char *ip = argv[1];
  int port = atoi(argv[2]);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0)
  {
    perror("inet_pton");
    exit(1);
  }

  int clientsQtd;

  printf("How many clients: ");
  scanf("%d", &clientsQtd);

  int sockfd[MAX_CLIENTS];

  for (int i = 0; i < clientsQtd; i += 1)
  {
    sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);

    printf("Cliente %d tentando conectar.\n", i + 1);

    Connect(sockfd[i], (struct sockaddr *)&servaddr, sizeof(servaddr));
  }

  for (int i = 0; i < clientsQtd; i += 1)
  {
    close(sockfd[i]);
  }

  exit(0);
}
