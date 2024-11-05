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
  int sockfd1, sockfd2;
  char buff1[1024] = {0};
  char buff2[1024] = {0};
  char error[MAXLINE + 1];
  struct sockaddr_in servaddr1, servaddr2;
  fd_set readfds;
  int max_fd;

  // verifica se foi passado o ip e a porta para o servidor de processamento se conectar
  if (argc != 5)
  {
    fprintf(stderr, "Usage: %s <ip> <port1> <port2> <entrada>\n", argv[0]);
    perror(error);
    exit(1);
  }

  char *ip = argv[1];
  int port1 = atoi(argv[2]);
  int port2 = atoi(argv[3]);
  char *input_file = argv[4];

  sockfd1 = Socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr1, sizeof(servaddr1));
  servaddr1.sin_family = AF_INET;
  servaddr1.sin_port = htons(port1);
  if (inet_pton(AF_INET, ip, &servaddr1.sin_addr) <= 0)
  {
    perror("inet_pton");
    exit(1);
  }

  sockfd2 = Socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr2, sizeof(servaddr2));
  servaddr2.sin_family = AF_INET;
  servaddr2.sin_port = htons(port2);
  if (inet_pton(AF_INET, ip, &servaddr2.sin_addr) <= 0)
  {
    perror("inet_pton");
    exit(1);
  }

  FD_ZERO(&readfds);
  FD_SET(sockfd1, &readfds);
  FD_SET(sockfd2, &readfds);
  max_fd = (sockfd1 > sockfd2) ? sockfd1 : sockfd2;

  Connect(sockfd1, (struct sockaddr *)&servaddr1, sizeof(servaddr1));
  Connect(sockfd2, (struct sockaddr *)&servaddr2, sizeof(servaddr2));

  int valread1;
  int valread2;

  for (;;)
  {
    // Copiando o conjunto de descritores para evitar modificações
    fd_set tmp_fds = readfds;

    if (select(max_fd + 1, &tmp_fds, NULL, NULL, NULL) < 0) {
        break;
    }

    // // Verificando se há dados para ler em cada socket
    if (FD_ISSET(sockfd1, &tmp_fds)) {
    //         // Ler dados do servidor 1
    //         // ...
            // char *message1 = "Hello from client to server 1";
            // send(sockfd1, message1, strlen(message1), 0);
            
            FILE *fp = fopen(input_file, "r");
            if (fp == NULL) {
                perror("fopen");
                exit(1);
            }

            char buffer1[1024];
            while (fgets(buffer1, sizeof(buffer1), fp) != NULL) {
                // Remove o caractere de nova linha, se houver
                // buffer[strcspn(buffer, "\n")] = 0;

                // Envia os dados para o servidor
                send(sockfd1, buffer1, strlen(buffer1), 0);
            }

            valread1 = read(sockfd1, buff1, 1024);
            if (valread1 == 0)
            {
              shutdown(sockfd1, SHUT_WR);
              break;
            }
            printf("%s\n", buff1);
    }
    if (FD_ISSET(sockfd2, &tmp_fds)) {
    //         // Ler dados do servidor 2
    //         // ...
            // char *message2 = "Hello from client to server 2";
            // send(sockfd2, message2, strlen(message2), 0);

            FILE *fp = fopen(input_file, "r");
            if (fp == NULL) {
                perror("fopen");
                exit(1);
            }

            char buffer2[1024];
            while (fgets(buffer2, sizeof(buffer2), fp) != NULL) {
                // Remove o caractere de nova linha, se houver
                // buffer[strcspn(buffer, "\n")] = 0;

                // Envia os dados para o servidor
                send(sockfd2, buffer2, strlen(buffer2), 0);
            }

            valread2 = read(sockfd2, buff2, 1024);
            if (valread2 == 0)
            {
              shutdown(sockfd2, SHUT_WR);
              break;
            }
            printf("%s\n", buff2);
    }
  }

  Close(sockfd1);
  Close(sockfd2);

  exit(0);
}
