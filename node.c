#include "node.h"

struct node * node_new(){
  struct node * n =NULL;

  n=malloc(sizeof(struct node));

  scn++;
  n->id=scn;
  n->attributes=NULL;
  n->relations=NULL;
  n->dirty=1;
  n->deleted=0;
  n->file_id=-1;
  n->file_pos=-1;
  
  return n;
}

int node_display(int sck, struct node * n){
  char * tmp;

  tmp=malloc(strlen("Node : ")+12); //very large values of ID
  bzero(tmp,strlen("Node : ")+12);
  sprintf(tmp,"Node : %ld\n",n->id);
  write(sck, tmp, strlen(tmp));
  free (tmp);
  attributes_display(sck, n->attributes);
  return 0;
  
}

int nodes_display(int sck, struct dll * nodes){
  struct node * n=NULL;

  if(nodes==NULL){
    write(sck, "No Nodes defined yet\n", strlen("No Nodes defined yet\n"));
  } else {
    nodes=dll_first(nodes);
    while(nodes->next!=NULL){
      node_display(sck, nodes->payload);
      nodes=nodes->next;
    }
    node_display(sck, nodes->payload);
  }
  
  return 0; 
}

struct node * node_search_by_id(struct dll * nodes, long int id){

  struct node * n, *t;
  
  if (nodes==NULL){
    n=NULL;
  } else{
    nodes=dll_first(nodes);
    while(nodes->next!=NULL){
      t=nodes->payload;
      if(t->id==id) n=nodes->payload;
      nodes=nodes->next;
    }
    t=nodes->payload;
    if(t->id==id) n=nodes->payload;
  }
  return n;
}

struct dll * node_search_by_attribute(struct dll * resultset, struct base * b, char * key, char * value){
  struct node * n;
  int match;
  
  resultset=NULL;
  
  if(b!=NULL){
    if(b->nodes!=NULL){
      b->nodes=dll_first(b->nodes);
      while(b->nodes->next!=NULL){
	n=b->nodes->payload;
	
	match=attributes_match(n->attributes, key, value);
	if(match==1) resultset=dll_add(resultset, n);
	b->nodes=b->nodes->next;
      }
    }
    n=b->nodes->payload;
    match=attributes_match(n->attributes,key,value);
    if(match==1) resultset=dll_add(resultset, n);
  }  

  return resultset;
}
  
