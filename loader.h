#ifndef _LOADER_
#define _LOADER_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "base.h"
#include "attribute.h"
#include "node.h"
#include "dll.h"

extern int state;
extern struct dll * bases;

int graph_loader();
void * read_block_map(void *, char *);
int loader(char *);
int graph_loader_base(void *, int, int);
int graph_loader_base_attribute(void *, int, int);
int graph_loader_node(void *, int, int);
int graph_loader_node_attribute(void *, int, int);
int graph_loader_relation(void *, int, int);
int graph_loader_relation_attribute(void *, int, int);
#endif
