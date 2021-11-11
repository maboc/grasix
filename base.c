#include "base.h"

/*
  Returns an empty base 
  No attributes, no nodes
*/

struct base * base_new(){
  struct base * b;
  
  b=malloc(sizeof(struct base));
  scn++;
  b->id=scn;
  b->nodes=NULL;
  b->attributes=NULL;
  b->dirty=1;
  b->deleted=0;
  b->file_id=-1;
  b->file_pos=-1;
  return b;
}

int base_list(int sck, struct base *b, int full){//full : 0 - flat list 1 - also show nodes
  char * tmp;
  int n;

  tmp=malloc(32);
  tmp=memset(tmp,0, 32);
  sprintf(tmp,"------------------------------\n");
  write(sck, tmp, strlen(tmp));
  free(tmp);
  
  tmp=malloc(strlen("Base : %ld\n")+12); //for very large ID
  bzero(tmp,strlen("Base : %ld\n")+12);
  sprintf(tmp, "Base : %ld\n", b->id);
  write(sck, tmp, strlen(tmp));
  free(tmp);
  attributes_display(sck, b->attributes);
  n=dll_count(b->nodes);
  tmp=malloc(strlen("Nodes : %ld\n")+12); //adding 12 for very large integers
  sprintf(tmp, "Nodes : %ld\n", n);
  write(sck, tmp, strlen(tmp));
  free(tmp);

  if(full==1) nodes_display(sck, b->nodes);//appreantly we also have to show the nodes
  
  return 0;
}

int bases_list(int s){
  if(bases==NULL){
    write(s, "No bases defined yet\n", strlen("No bases defined yet\n"));
  } else {
    bases=dll_first(bases);
    while (bases->next!=NULL){
      base_list(s, bases->payload, 0);
      bases=bases->next;
    }
    base_list(s, bases->payload, 0);
  }
  return 0;
}

struct base * base_search_by_id(int bid){
  struct base * b=NULL, * t=NULL;
  
  if (bases==NULL){
    b=NULL;
  } else {
    bases=dll_first(bases);
    while (bases->next!=NULL){
      t=bases->payload;
      if (t->id==bid) b=t;
      bases=bases->next;
    }
    t=bases->payload;
    if (t->id==bid) b=t;
  }
  
  return b;
}

int base_display(int sck, int id){
  struct base * b;

  b=base_search_by_id(id);
  if(b!=NULL)base_list(sck, b, 1);

  return 0;  
}
