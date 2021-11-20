#ifndef _RELATION_
#define _RELATION_

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "struct.h"
#include "dll.h"
#include "attribute.h"

extern long int scn;

struct relation * relation_new();
int relations_display(int, struct dll *);
int relation_display(int, struct relation *);
struct relation * relation_search_by_id(struct dll *, long int);

#endif
