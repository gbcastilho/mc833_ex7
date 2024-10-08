#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "net_api.h"

void SendReq(int sockfd, int fd, const char *msg, const char *ipstr, uint16_t port);

void ReceiveRes(int sockfd, int fd, const char *ipstr, uint16_t port);