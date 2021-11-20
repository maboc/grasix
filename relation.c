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
  struct node * relates_to;

  relates_to=r->relates_to; //for some reason r->relates_to->id did not work....do a step in between
  
  tmp=malloc(strlen("Relation : relates to : \n")+24); //very large values of ID
  bzero(tmp,strlen("Relation : relates to : \n")+24);
  sprintf(tmp,"Relation : %ld : relates to : %ld\n",r->id, relates_to->id);
  write(sck, tmp, strlen(tmp));
  free(tmp);
  
  attributes_display(sck, r->attributes);

  return 0;
}

struct relation * relation_search_by_id(struct dll * relations, long int id){

  struct relation * r, *t;
  
  if (relations==NULL){
    r=NULL;
  } else{
    relations=dll_first(relations);
    while(relations->next!=NULL){
      t=relations->payload;
      if(t->id==id) r=relations->payload;
      relations=relations->next;
    }
    t=relations->payload;
    if(t->id==id) r=relations->payload;
  }
  return r;
}
