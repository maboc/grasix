#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "network.h"
#include "worker.h"
#include "loader.h"
#include "command.h"
#include "node.h"
#include "base.h"
#include "attribute.h"
#include "writer.h"


extern long int scn;
long int scn=0;

/*
  doorgaan:
  0 - keep on going
  1 - stop the system
*/
extern int doorgaan;
int doorgaan=0;

/*
  state:
  0 - graph is closed
  1 - graph is building
  2 - graph is loaded
  3 - graph is opened
*/
extern int state;
int state=0;

extern struct dll * bases;
struct dll * bases=NULL;
