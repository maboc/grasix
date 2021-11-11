#ifndef _COMMAND_
#define _COMMAND_

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "dll.h"

struct dll * parse(char *);
char * remove_leading_spaces(char *);
char * remove_trailing_spaces(char *);
struct dll * breakup(char *);
char * command_part(struct dll *, int);
void command_free(struct dll *);

#endif
