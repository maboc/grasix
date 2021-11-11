#include "loader.h"

int graph_loader(){
  char * buffer;
  int file_size=1024*1024;
  int n;

  void * tmp_block=NULL;
  
  //  sleep(20);
  
  state=1;

  loader("a");
  loader("b");
  loader("c");
  loader("d");

  return 0;
}

int loader(char * t){
  char * file_name;
  FILE * fp;
  void * block_map=NULL;
  int block_map_pos;
  void * block=NULL;
  
  file_name =malloc(strlen("grasix.dat")+1);
  bzero(file_name, strlen("grasix.dat")+1);
  file_name=strncpy(file_name, "grasix.dat", strlen("grasix.dat"));
  
  if(access(file_name, F_OK)==0){
    printf("Can access file %s\n", file_name);
    
    block_map=read_block_map(block_map, file_name);//first read the block map
    block_map_pos=0;

    fp=fopen(file_name, "rb");
    free(file_name);
    
    while(block_map_pos<1024){//ah great...another hardcoded number
      char * block_code=NULL;
      block_code=malloc(2);
      block_code=memset(block_code,0,2);
      block_code=strncpy(block_code, block_map+block_map_pos, 1); //read the code from block_map position pos

      block=malloc(512);//Hardcodes sheit
      block=memset(block, 0, 512);
      fseek(fp, 1024+block_map_pos*512, SEEK_SET);
      fread(block, 512, 1, fp);

      //the actual loading will now take place
      if(strncmp(t, block_code, 1)==0){
	if(strncmp(block_code, "a", 1)==0){
	  graph_loader_base(block, 1, block_map_pos);
	} else if(strncmp(block_code, "b", 1)==0){
	  graph_loader_base_attribute(block, 1, block_map_pos);
	} else if(strncmp(block_code, "c", 1)==0){
	  graph_loader_node(block, 1, block_map_pos);
	}     else if(strncmp(block_code, "d", 1)==0){
	  graph_loader_node_attribute(block, 1, block_map_pos);
	}
      }
      free(block);
      free(block_code);
      block_map_pos++;//advance the postion to read to the next position					 
    }
    free(block_map);
    fclose(fp);
  } else {
    printf("Cannot access file %s\n", file_name);
  }
  
  state=2;
  return 0;
}

void * read_block_map(void * block_map, char * file_name){
  FILE * fp;
  
  fp=fopen(file_name, "rb");

  block_map=malloc(1024);//Ow  ... nice ....more hardcoded numbers
  block_map=memset(block_map,0,1024);

  //now read the block_map
  fseek(fp,0, SEEK_SET);
  fread(block_map, 1024, 1, fp);

  return block_map;
}

int graph_loader_base(void * block, int file_id, int file_pos ){

  struct base * b;
  long int tmp;
  
  memcpy(&tmp, block, sizeof(long int)); //only thing to read is the actual base id
  
  b=base_new();
  
  b->id=tmp;
  b->nodes=NULL;
  b->attributes=NULL;
  b->dirty=0;
  b->deleted=0;
  b->file_id=file_id;
  b->file_pos=file_pos;
  bases=dll_add(bases, b);
  
  return 0;
}

int graph_loader_base_attribute(void * block, int file_id, int file_pos){
  
  struct base * b;
  struct attribute * a=NULL;
  long int tmp, base_id, attribute_id;
  int l;
  char * key, * value;
  
  //let start by reading the ID of this attribute
  memcpy(&tmp, block, sizeof(long int));
  attribute_id=tmp;
  
  block=block+sizeof(long int);//advance the pointer
  memcpy(&tmp, block, sizeof(long int));
  base_id=tmp;
  
  block=block+sizeof(long int);//advance the pointer. W're now at the positon for the key of the attribute
  l=strlen(block);
  key=malloc(l);
  key=strncpy(key, block, strlen(block));
  
  block=block+strlen(block)+1;//advance the pointer. W're now at the position of the value of the attribute
  l=strlen(block);
  value=malloc(strlen(block));
  value=strncpy(value, block, strlen(block));
  
  a=attribute_new(key, value);
  a->id=attribute_id;
  a->dirty=0;
  a->deleted=0;
  a->file_id=file_id;
  a->file_pos=file_pos;
  
  b=base_search_by_id(base_id);//search the base 
	
  b->attributes=dll_add(b->attributes, a);//add the attribute to the base

  return 0;
}

int graph_loader_node(void * block, int file_id, int file_pos){
  //ah....a node
  struct base * b=NULL;
  struct node * n=NULL;
  long int tmp, base_id, node_id;
	

  //let start by reading the ID of this node
  memcpy(&tmp, block, sizeof(long int));
  node_id=tmp;
	
  block=block+sizeof(long int);//advance the pointer
  memcpy(&tmp, block, sizeof(long int));//now read the ID of the base this node belongs to
  base_id=tmp;

  n=node_new();
  n->dirty=0;
  n->deleted=0;
  n->file_id=file_id;
  n->file_pos=file_pos;
  n->id=node_id;
	
  b=base_search_by_id(base_id);
  b->nodes=dll_add(b->nodes, n);
	
  return 0;
}

int graph_loader_node_attribute(void * block, int file_id, int file_pos){
  //ah....an attribute belonging to a node
  struct base * b;
  struct node * n;
  struct attribute * a=NULL;
  long int tmp, base_id, node_id, attribute_id;
  int l;
  char * key, * value;
		
  //let start by reading the ID of this attribute
  memcpy(&tmp, block, sizeof(long int));
  attribute_id=tmp;
	
  block=block+sizeof(long int);//advance the pointer
  memcpy(&tmp, block, sizeof(long int));
  base_id=tmp;

  block=block+sizeof(long int);//advance the pointer
  memcpy(&tmp, block, sizeof(long int));
  node_id=tmp;

  block=block+sizeof(long int);//advance the pointer. W're now at the positon for the key of the attribute
  l=strlen(block);
  key=malloc(l);
  key=strncpy(key, block, strlen(block));
  
  block=block+strlen(block)+1;//advance the pointer. W're now at the position of the value of the attribute
  l=strlen(block);
  value=malloc(strlen(block));
  value=strncpy(value, block, strlen(block));
  
  a=attribute_new(key, value);
  a->id=attribute_id;
  a->dirty=0;
  a->deleted=0;
  a->file_id=file_id;
  a->file_pos=file_pos;
	
  b=base_search_by_id(base_id);//search the base 
  n=node_search_by_id(b->nodes, node_id);

  n->attributes=dll_add(n->attributes, a);
	
  return 0;
}
