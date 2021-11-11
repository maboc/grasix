#ifndef _STRUCT_
#define _STRUCT_

struct dll {
  struct dll * prev;
  struct dll * next;
  void * payload;
};


struct node {
  long int id;
  struct dll * attributes;
  struct dll * relations;
  int dirty;  //0: clean 1:dirty
  int deleted; //0: it's there 1: deleted
  int file_id;
  int file_pos;
};

struct base {
  long int id;
  struct dll * attributes;
  struct dll * nodes;
  int dirty;  //0: clean 1:dirty
  int deleted; //0: it's there 1: deleted
  int file_id;
  int file_pos;
};

struct attribute {
  long int id;
  char * key;
  char * value;
  int dirty;  //0: clean 1:dirty
  int deleted; //0: it's there 1: deleted
  int file_id;
  int file_pos;
};

#endif
