#include "server.h"

void SendReq(int sockfd, int fd, const char *msg, const char *ipstr, uint16_t port)
{
  char buf[MAXDATASIZE];
  memset(buf, 0, sizeof(buf));

  snprintf(buf, sizeof(buf), "%s", msg);
  Send(sockfd, buf, strlen(buf), 0);
  snprintf(buf, sizeof(buf), "%s ENVIADA PARA O SERVIDOR DE PROCESSAMENTO IP: %s PORTA: %d\n", msg, ipstr, port);
  if (write(fd, buf, strlen(buf)) == -1)
  {
    perror("write");
    exit(1);
  }
}

void ReceiveRes(int sockfd, int fd, const char *ipstr, uint16_t port)
{
  char recv_buf[MAXDATASIZE], buf[MAXDATASIZE];
  memset(recv_buf, 0, sizeof(recv_buf));
  memset(buf, 0, sizeof(buf));

  Recv(sockfd, recv_buf, MAXDATASIZE, 0);
  snprintf(buf, sizeof(buf), "%.*s PELO SERVIDOR DE PROCESSAMENTO IP: %s PORTA: %d\n", (int)strlen(recv_buf), recv_buf, ipstr, port);
  if (write(fd, buf, strlen(buf)) == -1)
  {
    perror("write");
    exit(1);
  }
}