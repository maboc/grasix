#include "writer.h"

void * graph_writer(void * input){
  struct base * b;
  int ret;
  
  sleep(30); //just for debugging purposes
  while((doorgaan==0) && (state ==2)){
    sleep(3);//we don't need to write instantaneous
    if(bases!=NULL){
      bases=dll_first(bases);
      while(bases->next!=NULL){
	ret=write_base(bases->payload);
	bases=bases->next;
      }
      ret=write_base(bases->payload);
    }
  }
}

int write_base(struct base * b){
  int free_pos=0;

  if(b->dirty==1){
    if(b->deleted==0){
      if((b->file_id==-1 )||(b->file_pos==-1)){
	b->file_id=1;
	printf("graph_writer : write_base : File is harcoded to 1\n");
	
	//get the first free position in the block_map
	free_pos=free_pos_search(b->file_id);
	b->file_pos=free_pos;
      }
      //write the block_map
      write_block_map(b->file_pos, "a");
      
      //write the content	
      void * block=NULL; //I do not understand why I have to decalre block here...but otherwise it would not zero
      block=malloc(512); //standard block size
      //	bzero(block, 512);
      block=memset(block, 0, 512);
      block=memcpy(block, & b->id, sizeof(long int));
      write_block(b->file_pos, block);
      free(block);
    } else {
      printf("deleted....not handled yet\n");
    }
    b->dirty=0;
  }
  //Now write the attributes belonging to this base
  b->attributes=write_base_attributes(b, b->attributes);
  
  //And write the nodes (the attributes of the nodes are handled recusrvely)
  b->nodes=write_nodes(b, b->nodes);
  
  return 0;
}

struct dll * write_base_attributes(struct base * b, struct dll * attributes){
  int ret;
  
  if(attributes!=NULL){
    attributes=dll_first(attributes);
    while(attributes->next!=NULL){
      ret=write_base_attribute(b, attributes->payload);
      attributes=attributes->next;
    }
    ret=write_base_attribute(b, attributes->payload);
  }
  return attributes;
}

int write_base_attribute(struct base * b, struct attribute * a){
  int free_pos=0;

  if(a->dirty==1){
    if(a->deleted==1){
      write_block_map(a->file_pos,"\0"); //write away a zero in the file

      //handling of the actual deletion is not yet dealed with...however a->deleted=1  ....so this attribute will not show upp in results
      
    } else {
      if((a->file_id==-1 )||(a->file_pos==-1)){
	a->file_id=1;
	printf("graph_writer : write_base_attribtue : File is harcoded to 1\n");
	
	//get the first free position in the block_map
	free_pos=free_pos_search(a->file_id);
	a->file_pos=free_pos;
      }	
      //write the block_map
      write_block_map(a->file_pos, "b");
      
      //write the content	
      void * block=NULL; //Do not understand why I have to declare the block here????
      void *tmp=NULL;
      block=malloc(512); //standard block size
      tmp=block;
      block=memset(block, 0, 512);
      block=memcpy(block, & a->id, sizeof(long int));//copy the ID of the attribute;
      block=block+sizeof(long int); //advance to the next free position in th block
      block=memcpy(block, & b->id, sizeof(long int));//copy the ID of the base
      block=block+sizeof(long int); //advance to the next free position in th block
      block=strncpy(block, a->key, strlen(a->key)+1); //copy the key of the attribute to the block
      block=block+strlen(a->key)+1;//advance to the next free position (+1 for the terminating zero)
      block=strncpy(block, a->value, strlen(a->value)+1); //copy the key of the attribute to the block
      write_block(a->file_pos, tmp);//block is een verschove positie :-) schrijg tmp weg!
      free(tmp);
    }
    a->dirty=0; 
  }
  return 0;
}

struct dll * write_nodes(struct base * b, struct dll * nodes){

  if(nodes!=NULL){
    nodes=dll_first(nodes);
    while(nodes->next!=NULL){
      write_node(b, nodes->payload);
      nodes=nodes->next;
    }
    write_node(b, nodes->payload);
  }
  
  return nodes;
}

int write_node(struct base * b, struct node * n){
  int free_pos;
  
  if(n->dirty==1){
    if(n->deleted==1){
      printf("NOde deletion is not handled...yet\n");
    } else {
      //apparently this node has newver been saved....find a free position in the designated file
      if((n->file_id==-1)||(n->file_pos==-1)){
	n->file_id==1;//hardcoded sheit
	free_pos=free_pos_search(n->file_id);
	n->file_pos=free_pos;
      }

      write_block_map(n->file_pos, "c");//write a c in the block map signifying a node

      //write the content
      void * block=NULL;
      void * tmp=NULL;
      block=malloc(512);//also hardcoded blocksize
      block=memset(block, 0,512);//also hardcoded blocksize
      tmp=block;
      
      block=memcpy(block, & n->id, sizeof(long int));//copy the ID of the node;
      block=block+sizeof(long int); //advance to the next free position in th block
      block=memcpy(block, & b->id, sizeof(long int));//copy the ID of the base in the block;
      
      write_block(n->file_pos, tmp);

      free(tmp);
    }
    
    n->dirty=0;
  }

  n->attributes=write_node_attributes(b, n, n->attributes);
  
  return 0;
}

struct dll* write_node_attributes(struct base * b, struct node * n, struct dll * attributes){
  struct dll * ret;
  
  if(attributes!=NULL){
    attributes=dll_first(attributes);

    while(attributes->next!=NULL){
      ret=write_node_attribute(b,n,attributes->payload);
      attributes=attributes->next;
    }
    ret=write_node_attribute(b,n,attributes->payload);
  }
  
  return attributes;
}

struct dll * write_node_attribute(struct base * b, struct node * n, struct attribute * a){
  int free_pos;
  
  if(a->dirty==1){
    if(a->deleted==1){
      //block_map aanpassen en wegschrijven
      write_block_map(a->file_pos, "\0");// signifying there is nothing there (lazy ...no block cleanup) 
      
      //actually delete the attribute
      //n->attributes=attribute_delete(n->attributes, a);
      
    } else {
      if((a->file_id==-1 )||(a->file_pos==-1)){
	a->file_id=1;
	printf("graph_writer : write_node_attribtue : File is harcoded to 1\n");
	
	//get the first free position in the block_map
	free_pos=free_pos_search(a->file_id);
	a->file_pos=free_pos;
      }	
      //write the block_map
      write_block_map(a->file_pos, "d");//d signifying an attribute of a node 
      
      //write the content	
      void * block=NULL; //Do not understand why I have to declare the block here????
      void * tmp=NULL;
      block=malloc(512); //standard block size
      tmp=block;
      block=memset(block, 0, 512);
      block=memcpy(block, & a->id, sizeof(long int));//copy the ID of the attribute;
      block=block+sizeof(long int); //advance to the next free position in th block
      block=memcpy(block, & b->id, sizeof(long int));//copy the ID of the base
      block=block+sizeof(long int); //advance to the next free position in th block
      block=memcpy(block, & n->id, sizeof(long int));//copy the id of the node in the block
      block=block+sizeof(long int);//advance the pointer 
      block=strncpy(block, a->key, strlen(a->key)+1); //copy the key of the attribute to the block
      block=block+strlen(a->key)+1;//advance to the next free position (+1 for the terminating zero)
      block=strncpy(block, a->value, strlen(a->value)+1); //copy the key of the attribute to the block
      write_block(a->file_pos, tmp);//block is een verschoven positie :-) schrijf tmp weg!
      free(tmp);
    }
    a->dirty=0;       
  }
  
  return n->attributes;
}

int free_pos_search(int file_id){
  FILE * fp;
  char * file_name;
  char * block_map;
  int pos;
  
  file_name=malloc(strlen("grasix.dat")+1);
  bzero(file_name, strlen("grasix.dat")+1);
  file_name=strncpy(file_name, "grasix.dat", strlen("grasix.dat"));
  fp=fopen(file_name, "r");
  free(file_name);
  
  block_map=malloc(1024);//this is hardcoded :=-( Has to be a function of the filesize and the size of a block
  bzero(block_map,1024);

  fseek(fp, 0, SEEK_SET); //seek from the start of the file
  fread(block_map, 1024, 1, fp);

  //search the first NULL in the block_map;
  pos=0;
  while(*(block_map+pos)!='\0') {
    pos++;
  }

  fclose(fp);
  free(block_map);
  
  return pos;  		    
}

int write_block_map(int pos, char * type){
  FILE * fp;
  char * file_name;
  char * block_map;
  char * content;

  file_name=malloc(strlen("grasix.dat")+1);
  bzero(file_name, strlen("grasix.dat")+1);
  file_name=strncpy(file_name, "grasix.dat", strlen("grasix.dat"));
  fp=fopen(file_name, "rb+");
  free(file_name);
  
  block_map=malloc(1024);//this is hardcoded :=-( Has to be a function of the filesize and the size of a block
  bzero(block_map,1024);

  //first read the block_map from file to memorty
  fseek(fp, 0, SEEK_SET); //seek from the start of the file
  fread(block_map, 1024, 1, fp);

  //alter the block_map
  strncpy(block_map+pos, type, 1);
  
  //write the changed block_map
  fseek(fp, 0, SEEK_SET); //seek from the start of the file
  fwrite(block_map, 1024,1, fp);

  free(block_map);
  fclose(fp);
  return 0;
}


int write_block(int block_pos, void * block){
  FILE * fp;
  char * file_name;
  char * block_map;
  
  file_name=malloc(strlen("grasix.dat")+1);
  bzero(file_name, strlen("grasix.dat")+1);
  file_name=strncpy(file_name, "grasix.dat", strlen("grasix.dat"));
  fp=fopen(file_name, "rb+");
  free(file_name);

  //write the block **** All that hardcoded shit should be changed at some point
  fseek(fp, 1024+block_pos*512, SEEK_SET); //seek the block position from the start of the file 
  fwrite(block, 512,1, fp);
  fclose(fp);

  return 0;  
}
