#ifndef _DLL_
#define _DLL_

#include <stddef.h>
#include <stdlib.h>
#include "struct.h"

struct dll * dll_new();
struct dll * dll_add(struct dll*, void *);
struct dll * dll_last(struct dll *);
struct dll * dll_first(struct dll *);
int dll_count(struct dll *);

#endif
