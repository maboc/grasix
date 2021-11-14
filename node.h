#ifndef _NODE_
#define _NODE_

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "struct.h"
#include "dll.h"
#include "base.h"
#include "attribute.h"
#include "relation.h"

extern long int scn;

struct node * node_new();
int node_display(int s, struct node *);
int nodes_display(int sck, struct dll *);
struct node * node_search_by_id(struct dll *, long int);
struct dll * node_search_by_attribute(struct dll *, struct base *, char *, char *);
#endif
