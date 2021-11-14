#include "relation.h"

struct relation * relation_new(){

  /*
    struct relation {
    long int id; //if of the relation
    struct node * relates_to;
    struct dll * attributes;
    int dirty; //0: clean 1:dirty
    int deleted; //0: it's there 1: delted
    int file_id;
    int file_pos;
  */

  struct relation * r;

  r=malloc(sizeof(struct relation));
  //  memset(r, 0, sizeof(struct relation));
  scn++;
  r->id=scn;
  r->relates_to=NULL;
  r->attributes=NULL;
  r->dirty=1;
  r->deleted=0;
  r->file_id=-1;
  r->file_pos=-1;

  return r;  
}

int relations_display(int sck, struct dll * relations){
  
  if(relations!=NULL){
    relations=dll_first(relations);
    while (relations->next!=NULL){
      relation_display(sck, relations->payload);
      relations=relations->next;
    }
    relation_display(sck, relations->payload);
  } else {
    write(sck, "No relations define (yet)\n", strlen("No relations define (yet)\n"));
  }
}

int relation_display(int sck, struct relation * r){
  char * tmp;

  tmp=malloc(strlen("Relation : ")+12); //very large values of ID
  bzero(tmp,strlen("Relation : ")+12);
  sprintf(tmp,"Relation : %ld\n",r->id);
  write(sck, tmp, strlen(tmp));
  free (tmp);
  attributes_display(sck, r->attributes);

  return 0;
}
