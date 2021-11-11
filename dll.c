#include "dll.h"

struct dll * dll_new(){
  struct dll * d=NULL;

  d=malloc(sizeof(struct dll));
  d->prev=NULL;
  d->next=NULL;
  d->payload=NULL;

  return d;
}

struct dll * dll_add(struct dll * list, void * payload){
  struct dll * item;

  item=dll_new();
  item->payload=payload;
  
  if(list!=NULL){
    list=dll_last(list);
    list->next=item;
    item->prev=list;
  }
  return item;
}


struct dll * dll_last(struct dll * list){
  while(list->next!=NULL) list=list->next;

  return list;
}

struct dll * dll_first(struct dll * list){
  while(list->prev!=NULL) list=list->prev;

  return list;
}

int dll_count(struct dll * list){
  int n=0;
  if (list==NULL){
    n=0;
  } else {
    n=1;
    list=dll_first(list);
    while(list->next !=NULL){
      n++;
      list=list->next;
    }
  }
  return n;
}
