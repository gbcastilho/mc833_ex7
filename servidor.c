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
#include <sys/select.h>
#include "net_api.h"

#define MAX_CLIENTS 20

int client_sockets[MAX_CLIENTS] = {0};

int main(int argc, char *argv[])
{
  // Receive port from cli args
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int port = atoi(argv[1]);

  int socket_fd, connection_fd, max_fd, activity;
  struct sockaddr_in address;
  int address_len = sizeof(address);
  char buffer[MAXLINE];
  fd_set read_fds;

  socket_fd = Socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  Bind(socket_fd, (struct sockaddr *)&address, sizeof(address));

  Listen(socket_fd, MAX_CLIENTS);

  struct sockaddr_in client_address;
  int client_address_len = sizeof(client_address);

  for (;;)
  {
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);
    max_fd = socket_fd;

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
      if (client_sockets[i] > 0)
      {
        FD_SET(client_sockets[i], &read_fds);
      }

      if (client_sockets[i] > max_fd)
      {
        max_fd = client_sockets[i];
      }
    }

    activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
    if ((activity < 0) && (errno != EINTR))
    {
      printf("select error\n");
    }

    if (FD_ISSET(socket_fd, &read_fds))
    {
      connection_fd = Accept(socket_fd, (struct sockaddr *)&address, (socklen_t *)&address_len);

      for (int i = 0; i < MAX_CLIENTS; i++)
      {
        if (client_sockets[i] == 0)
        {
          client_sockets[i] = connection_fd;
          break;
        }
      }
    }

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
      if (FD_ISSET(client_sockets[i], &read_fds))
      {
        int valread = read(client_sockets[i], buffer, MAXLINE);
        if (valread <= 0)
        {
          Getpeername(client_sockets[i], (struct sockaddr *)&client_address, (socklen_t *)&client_address_len);
          printf("Host disconnected, ip %s, port %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
          Close(client_sockets[i]);
          client_sockets[i] = 0;
        }
        else
        {
          Getpeername(client_sockets[i], (struct sockaddr *)&client_address, (socklen_t *)&client_address_len);
          buffer[valread] = '\0';
          printf("Client %s:%d : %s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), buffer);

          Send(client_sockets[i], buffer, strlen(buffer), 0);
        }
      }
    }
  }
}
