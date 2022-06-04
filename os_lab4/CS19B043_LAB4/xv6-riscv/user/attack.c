#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int g(int x) {
  return x+3;
}

int f(int x) {
  return g(x);
}

void main(void) {
  /*int pid = fork();
  if(pid==0){
    //THis is child process
    pcbread();
  }
  else{
    //This is parent process
    int temp = wait((int*)0);
    temp += 1;
    pcbread();
  }*/
  sleep(10);
  //testing(1,2,3,4,5);
  exit(0);
}
