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
#include <arpa/inet.h>
#include <inttypes.h>

#define MAXDATASIZE 100
#define MAXLINE 4096

void Close(int fd);

int Socket(int family, int type, int flags);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Send(int sockfd, const void *buf, size_t len, int flags);

void Recv(int fd, void *buf, size_t len, int flags);

pid_t Fork(void);

void Getsockname(int __fd, struct sockaddr *__restrict__ __addr, socklen_t *__restrict__ __len);

void Getpeername(int __fd, struct sockaddr *__restrict__ __addr, socklen_t *__restrict__ __len);

void PrintSocketInfo(int sockfd, int local);