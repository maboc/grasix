#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv){
  void * block=NULL;
  
  block=malloc(512); //standard block size                                                                                                            
  //      bzero(block, 512);                                                                                                                          
  block=memset(block, 1, 512);
  free(block);
  
  return 0;
}
