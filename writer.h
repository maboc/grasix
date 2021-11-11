#ifndef _WRITER_
#define _WRITER

#include "dll.h"
#include "base.h"
#include "node.h"
#include "attribute.h"

extern int doorgaan;
extern int state;
extern struct dll * bases;

void * graph_writer(void *);
struct dll * write_nodes(struct base *, struct dll *);
int write_node(struct base *, struct node *);
struct dll * write_node_attributes(struct base *, struct node *, struct dll *);
struct dll *  write_node_attribute(struct base *, struct node *, struct attribute *);
struct dll * write_base_attributes(struct base *, struct dll *);
int write_base_attribute(struct base*, struct attribute *);
int write_base(struct base *);
int free_pos_search(int);
int write_block_map(int, char *);
int write_block(int, void *);
#endif
