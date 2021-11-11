#ifndef _NETWORK_
#define _NETWORK_

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <pthread.h>
#include "worker.h"

extern int doorgaan;

int network();

#endif
