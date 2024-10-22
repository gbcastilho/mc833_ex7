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
#include "net_api.h"
#include "server.h"

int main(int argc, char **argv)
{
  // verifica se a porta do servidor foi passada como parametro
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <port> <backlog_size>\n", argv[0]);
    exit(1);
  }

  int port = atoi(argv[1]);

  int backlog = atoi(argv[2]);

  int listenfd, connfd;
  struct sockaddr_in servaddr;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  Listen(listenfd, backlog);

  for (;;)
  {
    connfd = Accept(listenfd, (struct sockaddr *)NULL, NULL);

    pid_t pid;

    if ((pid = Fork()) == 0)
    {
      Close(listenfd);
      struct sockaddr_in addr;
      socklen_t addrlen = sizeof(addr);
      char ipstr[INET_ADDRSTRLEN];

      Getpeername(connfd, (struct sockaddr *)&addr, &addrlen);
      inet_ntop(AF_INET, &(addr.sin_addr), ipstr, sizeof(ipstr));

      printf("Cliente (%s:%d) conectado.\n", ipstr, htons(addr.sin_port));

      Close(connfd);

      exit(0);
    }
    
    sleep(10);
    Close(connfd);
  }
  return (0);
}
