#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int n = 0;
  int len = strlen(argv[1]);
  int i=0;
  while(i<len){
    n *= 10;
    n += (argv[1][i++]-'0');
  }
  sleep(n);
  exit(0);
}
