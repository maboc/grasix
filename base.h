#ifndef _BASE_
#define _BASE_

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "struct.h"
#include "dll.h"
#include "attribute.h"
#include "node.h"

extern long int scn;
extern struct dll * bases;

struct base * base_new();
int base_list(int, struct base *, int);
int bases_list(int);
struct base * base_search_by_id(int);
int base_display(int, int);
#endif
