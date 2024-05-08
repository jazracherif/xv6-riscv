#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(2, "Usage: sleep numticks...\n");
    exit(1);
  }

  int numticks = atoi(argv[1]);

  fprintf(1, "Sleeping for %d ticks...\n", numticks);
  sleep(numticks);

  exit(0);
}
