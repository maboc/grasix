#ifndef _ATTRIBUTE_
#define _ATTRIBUTE_

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "struct.h"
#include "dll.h"

extern long int scn;

struct attribute * attribute_new(char *, char *);
int attribute_display(int, struct attribute *);
int attributes_display(int, struct dll *);
int attributes_match(struct dll *, char *, char *);
struct attribute * attribute_search_by_id(struct dll *, int);
struct dll * attribute_delete(struct dll *, struct attribute *);
int attribute_do_delete(struct dll *, struct attribute *);
struct dll * attributes_delete_all(struct dll *);
#endif
