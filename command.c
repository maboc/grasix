#include "command.h"

struct dll * parse(char * command_string){

  char * o1, * o2;
  struct dll * command=NULL;
  
  o1=remove_leading_spaces(command_string);
  o2=remove_trailing_spaces(o1);
    
  command=breakup(o2);

  free(o1);
  free(o2);
  
  return command;
}

char * remove_leading_spaces(char * input){
  char * output=NULL;

  while(* input == ' ') input++;
  output=malloc(strlen(input)+1); // +1 for a trailing \0
  bzero(output, strlen(input)+1);
  output=strcpy(output, input);

  return output;  
}

char * remove_trailing_spaces(char * input){
  char * output=NULL;
  char * ep; // pointer to the current end of the string
  char * tmp;

  ep=input+strlen(input)-1;
  while((*ep==' ' ) ||  (*ep=='\r') || (*ep=='\n')) {
    ep=ep-1;
  }
  output=malloc(ep-input+1+1); //+1 (off by one) +1 om een \0 te kunen toevoegen
  bzero(output, ep-input+1+1);
  output=strncpy(output, input, ep-input+1);

  return output;
}

struct dll* breakup(char * command_line){
  char * l, * tmp, * t;
  struct dll * c=NULL;

  t=command_line;
  printf("Breakup : strlen : %i\n", strlen(t));
  if(strlen(t)>0){
    l=strstr(t, " ");
    while (l!=NULL){
      tmp=malloc(l-t+1);
      bzero(tmp, l-t+1); 
      tmp=strncpy(tmp, t, l-t);
      c=dll_add(c, tmp);
      t=l+1;
      l=strstr(t, " ");
    }
    tmp=malloc(strlen(t)+1);
    bzero(tmp, strlen(t)+1);
    tmp=strncpy(tmp, t, strlen(t));
    c=dll_add(c, tmp);

  }

  return c;
}

char * command_part(struct dll * cmd, int n){
  char * tmp;
  int i;
  
  if (dll_count(cmd)<n){
    tmp=NULL;
  } else {
    cmd=dll_first(cmd);
    i=1;
    while(i<n){
      cmd=cmd->next;
      i++;
    }
    tmp=cmd->payload;
  }
  return tmp;
}

void command_free(struct dll * c){

  struct dll * next;
  
  if(c!=NULL){
    c=dll_first(c);
    while (c->next!=NULL){
      free(c->payload);
      next=c->next;
      free(c);
      c=next;
    }
    free(c->payload);
    free(c);
  } 
  return;
}
