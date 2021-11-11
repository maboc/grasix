#include "grasix.h"

int main(int argc, char ** argv){
  int pid;
  pthread_t thread;
  int thread_ret;

  
  printf("Welcome to grasix\n");
  printf("Starting Up our engines\n");

  printf("Moving to the background\n");
  pid=fork();
  if (pid==0){
    sleep(20);
    printf("Welcome to your inner child\n");
    doorgaan=0;
    state=0;
    graph_loader();
    thread_ret=pthread_create(&thread, NULL, graph_writer, NULL);
    network();
  } else {
    printf("The parent speaking\n");
    printf("PID of the child : %i\n", pid);
  }
  
  return 0;
}
