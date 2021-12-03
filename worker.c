#include "worker.h"

void * worker(void * sck){
  void * inp;
  int s;
  int quit=0;
  int shutdown=0;
  struct dll * cmd=NULL;
  int n;
  
  s=*(int*)sck;
  write(s,"Welcome\n", strlen("welcome\n"));

  inp=malloc(100);
  bzero(inp, 100);
  inp=strncpy(inp, "nothing", strlen("nothing"));
  while((quit!=1)&&(doorgaan==0)){
    bzero(inp, 100);
    read(s, inp, 99);
    write(s, inp, strlen(inp));
    
    if(strncmp(inp, "quit\r\n", strlen(inp))==0){
      quit=1;
    } else if(strncmp(inp, "shutdown\r\n", strlen(inp))==0){
      doorgaan=1;
      quit=1;
    } else {
      cmd=parse(inp);
      if(cmd==NULL){
	write(s,"syntax error\n", strlen("syntax error\n"));
      } else {
	for (n=1;n<=dll_count(cmd);n++){
	  printf("cmd %i  : %s\n", n, command_part(cmd, n));
	}
	
	/*Let 's start working */
	//base new
	if((strncmp(command_part(cmd,1),"base",strlen(command_part(cmd,1)))==0) && (strncmp(command_part(cmd,2), "new", strlen(command_part(cmd,2)))==0) && (dll_count(cmd)==4)){
	  printf("Add a base\n");
	  struct base * b;
	  struct attribute * a;
	  b=base_new();
	  a=attribute_new(command_part(cmd,3), command_part(cmd,4));
	  b->attributes=dll_add(b->attributes, a);
	  bases=dll_add(bases, b);
	}
	
	//base list
	if((strncmp(command_part(cmd,1),"base",strlen(command_part(cmd,1)))==0) && (strncmp(command_part(cmd,2), "list", strlen(command_part(cmd,2)))==0) && (dll_count(cmd)==2)){
	  printf("List bases\n");
	  bases_list(s);
	}
	
	//base display
	if((strncmp(command_part(cmd,1),"base",strlen(command_part(cmd,1)))==0) && (strncmp(command_part(cmd,2), "display", strlen(command_part(cmd,2)))==0) && (dll_count(cmd)==3)){
	  printf("Base display\n");
	  base_display(s, atoi(command_part(cmd, 3)));
	}
	
	//node new
	if((strncmp(command_part(cmd,1),"node",strlen(command_part(cmd,1)))==0) && (strncmp(command_part(cmd,2), "new", strlen(command_part(cmd,2)))==0) && (dll_count(cmd)==5)){
	  struct base * b=NULL;
	  struct node * n=NULL;
	  struct attribute * a=NULL;
	  
	  printf("Add a node\n");
	  b=base_search_by_id(atoi(command_part(cmd, 3)));
	  n=node_new();
	  a=attribute_new(command_part(cmd,4), command_part(cmd, 5));
	  n->attributes=dll_add(n->attributes, a);
	  b->nodes=dll_add(b->nodes, n);
	  
	  node_display(s, n);
	}

	//attribute add base
	if((strncmp(command_part(cmd,1),"attribute",strlen(command_part(cmd,1)))==0) && (strncmp(command_part(cmd,2), "add", strlen(command_part(cmd,2)))==0) && (strncmp(command_part(cmd,3), "base", strlen(command_part(cmd,3)))==0) &&  (dll_count(cmd)==6)){
	  printf("Add an attribute to a base\n");
	  struct base * b=NULL;
	  struct attribute * a=NULL;

	  b=base_search_by_id(atoi(command_part(cmd,4)));
	  if(b==NULL){
	    write(s,"This base does not exist\n", strlen("This base does not exist\n"));
	  }else{
	    a=attribute_new(command_part(cmd,5), command_part(cmd, 6));
	    b->attributes=dll_add(b->attributes, a);
	  }
	}

	//attribute  add node
	if((strncmp(command_part(cmd,1),"attribute",strlen(command_part(cmd,1)))==0) && (strncmp(command_part(cmd,2), "add", strlen(command_part(cmd,2)))==0) && (strncmp(command_part(cmd,3), "node", strlen(command_part(cmd,3)))==0) &&  (dll_count(cmd)==7)){
	  printf("Add an attribute to a node\n");
	  struct base * b=NULL;
	  struct node * n=NULL;
	  struct attribute * a=NULL;

	  b=base_search_by_id(atoi(command_part(cmd,4)));
	  n=node_search_by_id(b->nodes, atoi(command_part(cmd,5)));
	  if(b==NULL){
	    write(s,"This base does not exist\n", strlen("This base does not exist\n"));
	  }else if (n==NULL){
	    write(s,"This node does not exist\n", strlen("This node does not exist\n"));
	  }else{
	    a=attribute_new(command_part(cmd,6), command_part(cmd, 7));
	    n->attributes=dll_add(n->attributes, a);
	  }
	}

	//search node
	if((strncmp(command_part(cmd,1),"search",strlen(command_part(cmd,1)))==0) &&\
	   (strncmp(command_part(cmd,2), "node", strlen(command_part(cmd,2)))==0) &&\
	   (dll_count(cmd)==5)){
	  struct base * b=NULL;
	  struct dll * resultset =NULL;
	  
	  printf("Search a node\n");
	  b=base_search_by_id(atoi(command_part(cmd, 3)));
	  resultset=node_search_by_attribute(resultset, b, command_part(cmd, 4), command_part(cmd,5));
	  nodes_display(s, resultset);	  
	}
	
	//attribute delete node 
	if(
	   (strncmp(command_part(cmd,1),"attribute",strlen(command_part(cmd,1)))==0) && \
	   (strncmp(command_part(cmd,2), "delete", strlen(command_part(cmd,2)))==0) && \
	   (strncmp(command_part(cmd,3), "node", strlen(command_part(cmd,3)))==0) && \
	   (dll_count(cmd)==6)
	   ){
	  struct base * b=NULL;
	  struct node * n=NULL;
	  struct attribute * a=NULL;
	  long int i;
	  
	  printf("Delete an attribute of a node\n");
	  i=atoi(command_part(cmd, 4));
	  b=base_search_by_id(i);
	  if(b!=NULL){
	    i=atoi(command_part(cmd, 5));
	    n=node_search_by_id(b->nodes, i);
	    if(n!=NULL){
	      i=atoi(command_part(cmd, 6));
	      a=attribute_search_by_id(n->attributes, i);
	      
	      if (a!=NULL){
		a->dirty=1;
		a->deleted=1;
	      } else {
		write(s, "That attribute does not exist\n", strlen("That attribute does not exist\n"));
	      }
	    }else{
	      write(s, "That node does not exist\n", strlen("That node does not exist\n"));
	    }
	  } else {
	    write(s, "That base does not exist\n", strlen("That base does not exist\n"));
	  }
	}
      }
      
      //attribute delete base 
      if(
	 (strncmp(command_part(cmd,1),"attribute",strlen(command_part(cmd,1)))==0) && \
	 (strncmp(command_part(cmd,2), "delete", strlen(command_part(cmd,2)))==0) && \
	 (strncmp(command_part(cmd,3), "base", strlen(command_part(cmd,3)))==0) && \
	 (dll_count(cmd)==5)
	 ){
	struct base * b=NULL;
	struct attribute * a=NULL;
	long int i;
	
	printf("Delete an attribute of a base\n");
	i=atoi(command_part(cmd, 4));
	b=base_search_by_id(i);
	if(b!=NULL){
	  i=atoi(command_part(cmd, 5));
	  a=attribute_search_by_id(b->attributes, i);
	  if (a!=NULL){
	    a->dirty=1;
	    a->deleted=1;
	  } else {
	    write(s, "That attribute does not exist\n", strlen("That attribute does not exist\n"));
	  }
	} else {
	  write(s, "That base does not exist\n", strlen("That base does not exist\n"));
	}
      }

      //relation new
      if(
    	 (strncmp(command_part(cmd,1),"relation",strlen(command_part(cmd,1)))==0) && \
	 (strncmp(command_part(cmd,2), "add", strlen(command_part(cmd,2)))==0) && \
	 (dll_count(cmd)==7)
	 ){
	struct base * b;
	struct node * target;
	struct node * relates_to;
	struct attribute * a;
	struct relation * r;
	char * k;//key
	char * v;//value
	int i;
	
	printf("Create a relation\n");
	i=atoi(command_part(cmd, 3));
	b=base_search_by_id(i);
	if(b!=NULL){
	  i=atoi(command_part(cmd, 4));
	  target=node_search_by_id(b->nodes, i);
	  if (target!=NULL){
	    i=atoi(command_part(cmd, 5));
	    relates_to=node_search_by_id(b->nodes, i);
	    if (relates_to!=NULL){
	      r=relation_new();
	      r->relates_to=relates_to;
	      a=attribute_new(command_part(cmd, 6),command_part(cmd, 7));
	      r->attributes=dll_add(r->attributes, a);
	      target->relations=dll_add(target->relations, r);

	      // On the other end (the relates_to end) We need to add the target as a referrer
	      relates_to->referrers=dll_add(relates_to->referrers, target);
	    }
	  }
	}	
      }

      //delete attribute relation
      if(
	 (strncmp(command_part(cmd,1),"attribute",strlen(command_part(cmd,1)))==0) && \
	 (strncmp(command_part(cmd,2), "delete", strlen(command_part(cmd,2)))==0) && \
	 (strncmp(command_part(cmd,3), "relation", strlen(command_part(cmd,3)))==0) && \
	 (dll_count(cmd)==7)
	 ){
	struct base * b=NULL;
	struct node * n=NULL;
	struct attribute * a=NULL;
	struct relation * r=NULL;
	long int i;
	
	printf("Delete an attribute of a relation\n");
	i=atoi(command_part(cmd, 4));
	b=base_search_by_id(i);
	if(b!=NULL){
	  i=atoi(command_part(cmd, 5));
	  n=node_search_by_id(b->nodes, i);
	  if(n!=NULL){
	    i=atoi(command_part(cmd, 6));
	    r=relation_search_by_id(n->relations, i);
	    if(r!=NULL){
	      i=atoi(command_part(cmd, 7));
	      a=attribute_search_by_id(r->attributes, i);
	      if (a!=NULL){
		a->dirty=1;
		a->deleted=1;
	      } else {
		write(s, "That attribute does not exist\n", strlen("That attribute does not exist\n"));
	      }
	    } else {
	      write(s, "That relation does not exist\n", strlen("That relation does not exist\n"));
	    }
	  } else {
	    write(s, "That node does not exist\n", strlen("That node does not exist\n"));
	  }
	} else {
	  write(s, "That base does not exist\n", strlen("That base does not exist\n"));
	}
      }
      
      //attribute add relation
      if(
	 (strncmp(command_part(cmd,1),"attribute",strlen(command_part(cmd,1)))==0) && \
	 (strncmp(command_part(cmd,2), "add", strlen(command_part(cmd,2)))==0) && \
	 (strncmp(command_part(cmd,3), "relation", strlen(command_part(cmd,3)))==0) && \
	 (dll_count(cmd)==8)){

	printf("Add an attribute to a relation\n");
	struct base * b=NULL;
	struct node * n=NULL;
	struct relation * r=NULL;
	struct attribute * a=NULL;
	int i;

	//search the base
	i=atoi(command_part(cmd,4));
	b=base_search_by_id(i);
	if(b!=NULL){

	  //if the base exists : search the node
	  i=atoi(command_part(cmd,5));
	  n=node_search_by_id(b->nodes, i);
	  if (n!=NULL){

	    //id the node exists search the relation
	    i=atoi(command_part(cmd,6));
	    r=relation_search_by_id(n->relations, i);
	    if(r!=NULL){

	      //if the relation exists finally add the attribute
	      a=attribute_new(command_part(cmd,7), command_part(cmd, 8));
	      r->attributes=dll_add(r->attributes, a);
	      
	    } else {
	      write(s,"This relation does not exist\n", strlen("This relation does not exist\n"));
	    }
	  } else {
	    write(s,"This node does not exist\n", strlen("This node does not exist\n"));
	  }
	} else {
	  write(s,"This base does not exist\n", strlen("This base does not exist\n"));
	}
      }

      //delete a relation
      if(
	 (strncmp(command_part(cmd,1),"relation",strlen(command_part(cmd,1)))==0) && \
	 (strncmp(command_part(cmd,2), "delete", strlen(command_part(cmd,2)))==0) && \
	 (dll_count(cmd)==5)
	 ){
	struct base * b=NULL;
	struct node * n=NULL;
	struct relation * r=NULL;
	struct node * rt;//this node is related to
	long int i;
	
	printf("Delete a relation\n");
	i=atoi(command_part(cmd, 3));
	b=base_search_by_id(i);
	if(b!=NULL){
	  i=atoi(command_part(cmd, 4));
	  n=node_search_by_id(b->nodes, i);
	  if(n!=NULL){
	    i=atoi(command_part(cmd, 5));
	    r=relation_search_by_id(n->relations, i);
	    if(r!=NULL){
	      
	      //first delete the attributes of the relation
              r->attributes=attributes_delete_all(r->attributes);
	      
	      //then delete the relation itself
	      r->dirty=1;
	      r->deleted=1;

	      //on the other side the referrer must be deleted
	      rt=r->relates_to;
	      
	    } else {
	      write(s, "That relation does not exist\n", strlen("That relation does not exist\n"));
	    }
	  } else {
	    write(s, "That node does not exist\n", strlen("That node does not exist\n"));
	  }
	} else {
	  write(s, "That base does not exist\n", strlen("That base does not exist\n"));
	}
      }

      //display a node (by id)
      if(
	 (strncmp(command_part(cmd,1),"display",strlen(command_part(cmd,1)))==0) && \
	 (strncmp(command_part(cmd,2), "node", strlen(command_part(cmd,2)))==0) && \
	 (dll_count(cmd)==4)
	 ){
	struct base * b=NULL;
	struct node * n=NULL;
	long int i;
	
	printf("Display a node by ID\n");
	i=atoi(command_part(cmd, 3));
	b=base_search_by_id(i);
	if(b!=NULL){
	  i=atoi(command_part(cmd, 4));
	  n=node_search_by_id(b->nodes, i);
	  if(n!=NULL){
	    //if the node is found.....display it
	    node_display(s, n);
	  } else {
	    write(s, "That node does not exist\n", strlen("That node does not exist\n"));
	  }
	} else {
	  write(s, "That base does not exist\n", strlen("That base does not exist\n"));
	}
      }
      command_free(cmd);
    }
  }
  
  free(inp);
  
  close(s);
  pthread_exit(NULL);
}
