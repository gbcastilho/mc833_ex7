#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include "net_api.h"
#include "server.h"

#define LISTENQ 10

int main(int argc, char **argv)
{
  // verifica se a porta do servidor foi passada como parametro
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int port = atoi(argv[1]);

  int listenfd, connfd;
  struct sockaddr_in servaddr;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  char buf[1024];

  for (;;)
  {
    connfd = Accept(listenfd, (struct sockaddr *)NULL, NULL);

    time_t rawtime;
    struct tm * timeinfo;
    char buf_time[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buf_time, sizeof(buf_time), "%a %b %d %H:%M:%S %Y", timeinfo);

    int cpu = rand() % 101;
    int mem = rand() % 101;

    char status[10];
    strcpy(status, (rand() % 2 == 0) ? "Inativo" : "Ativo");

    snprintf(buf, sizeof(buf), "Monitoramento do servidor:\nIP: %d\nPorta: %d\n%s\nCPU: %d%%\nMemoria: %d%%\nStatus: %s\n", servaddr.sin_addr.s_addr, servaddr.sin_port, buf_time, cpu, mem, status);
    send(connfd, buf, strlen(buf), 0);

    char buf[1024];
    int val_read;

    for (;;)
    {
      val_read = read(connfd, buf, 1024);
      if (val_read == 0)
      {
        break;
      }
      printf("%s\n", buf);
    }

    Close(connfd);
    break;
  }
  return (0);
}
