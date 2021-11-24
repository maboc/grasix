#include "attribute.h"

struct attribute * attribute_new(char * key, char * value){

  struct attribute * a;

  a=malloc(sizeof(struct attribute));
  scn++;
  a->id=scn;

  a->key=malloc(strlen(key)+1);
  bzero(a->key, strlen(key)+1);
  a->key=strncpy(a->key, key, strlen(key));


  a->value=malloc(strlen(value)+1);
  bzero(a->value, strlen(value)+1);
  a->value=strncpy(a->value, value, strlen(value));

  a->dirty=1;
  a->deleted=0;
  a->file_id=-1;
  a->file_pos=-1;
  
  return a;
}

int attributes_display(int sck, struct dll * attributes){
  struct attribute * a;
  
  if (attributes==NULL){
    write(sck,"No attributes yet\n", strlen("No attributes yet\n"));
  } else {
    attributes=dll_first(attributes);
    while (attributes->next!=NULL){
      a=attributes->payload;
      if(a->deleted!=1) attribute_display(sck, attributes->payload);
      attributes=attributes->next;
    }
    a=attributes->payload;
    if(a->deleted!=1) attribute_display(sck, attributes->payload);
  }
  return 0;
}


int attribute_display(int sck, struct attribute * a){
  char * tmp;

  tmp=malloc(strlen("Attribute : ")+strlen(a->key)+strlen(" : ")+strlen(a->value)+strlen("\n")+12); //very large values of ID
  bzero(tmp,strlen("Attribute : ")+strlen(a->key)+strlen(" : ")+strlen(a->value)+12);
  sprintf(tmp,"Attribute : %ld : %s : %s\n",a->id, a->key, a->value);
  write(sck, tmp, strlen(tmp));
  free (tmp);
  return 0;
}


int attributes_match(struct dll * attributes, char * key, char * value){
  int r;
  struct attribute * a;
  
  r=0;
  if(attributes!=NULL){
    attributes=dll_first(attributes);
    while(attributes->next != NULL){
      a=attributes->payload;
      if((a->deleted!=1)&&(strncmp(a->key, key, strlen(key))==0) && (strncmp(a->value, value, strlen(value))==0 )) r=1;
      attributes=attributes->next;
    }
    a=attributes->payload;
    if((a->deleted!=1)&&(strncmp(a->key, key, strlen(key))==0) && (strncmp(a->value, value, strlen(value))==0)) r=1;
  }
  return r;
    
}

struct attribute * attribute_search_by_id(struct dll * attributes, int a_id){
  struct attribute * a=NULL;
  struct attribute * r=NULL;
    
  if(attributes!=NULL){
    attributes=dll_first(attributes);
    while(attributes->next!=NULL){
      a=attributes->payload;
      if(a->id==a_id) r=a;  
      attributes=attributes->next;
    }
    a=attributes->payload;
    if(a->id==a_id) r=a;  
  }

  return r;
}

struct dll * attribute_delete(struct dll * attributes, struct attribute * a){
  struct dll * prev, * next;
  int job_done=0;//if job_done changes to 1 the attribute is found and deleted
  
  if(attributes!=NULL){
    //search for the specific attribute in the attributes dll
    attributes=dll_first(attributes);
    
    while((attributes->next!=NULL)&&(job_done==0)){
      if(attributes->payload==a){//here we find the attribute in the list (dll)
	prev=attributes->prev;
	next=attributes->next;
	
	if(prev==NULL){
	  //next!=NULL; cause the while loop catches the next==NULL case
	  //this is the first item in the list;
	  attribute_do_delete(attributes, a);

	  attributes=next; //THe list off attributes now points to the (former) next attribute
	  attributes->prev=NULL; //The new attribute now is the firat one in the list
	}else{
	  //next!=NULL; cause the while loop catches the next==NULL case
	  //so this is an attribute somewhere in the middle of the list
	  attribute_do_delete(attributes, a);

	  prev->next=next;//the "man in the middle is gone" so bypass is
	  next->prev=prev;
	  attributes=prev;//attributes must be something
	}
	job_done=1;
      }
      attributes=attributes->next;
    }
    
    //Now we are on the last attribute in the list
    if((job_done==0)&&(attributes->payload==a)){
      
      prev=attributes->prev;
      if(prev==NULL){//we are on the last entry in the list (next==NULL) but we are also on the first item on the list (prev==NULL): ther is only one item in the list
	attribute_do_delete(attributes, a);
	attributes=NULL; //since there is nothing left of this list
      }else{ //Now we are at last entry of the list (and not also the first one)
	attribute_do_delete(attributes, a);
	
	attributes=prev;//since we're on the last entry on the list, and that one is deleted...the prev must be the last now
	attributes->next=NULL;
      }
    }
  }  
  return attributes;
}

int attribute_do_delete(struct dll * attributes, struct attribute * a){
  free(a->key);
  free(a->value);
  free(a);
  free(attributes);

  return 0;
}

struct dll * attributes_delete_all(struct dll * attributes){
  struct dll * tmp;
  struct attribute * a;
  
  tmp=attributes;
  if(attributes!=NULL){
    attributes=dll_first(attributes);

    while(attributes->next!=NULL){
      a=attributes->payload;
      a->dirty=1;
      a->deleted=1;

      attributes=attributes->next;
    }
    a=attributes->payload;
    a->dirty=1;
    a->deleted=1;
  }
  
  return tmp;
}
