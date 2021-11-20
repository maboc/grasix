#ifndef _WORKER_
#define _WORKER_

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "command.h"
#include "base.h"
#include "attribute.h"
#include "node.h"
#include "relation.h"

extern struct dll * bases;
extern int doorgaan;


void * worker(void *);

#endif
