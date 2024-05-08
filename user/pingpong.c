#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "Usage: pingpong\n");
    exit(1);
  }

  // Create 2 pipes
  int childPipe[2]; // for pipe file description
  int parentPipe[2]; // for pipe file description

  pipe(childPipe);
  pipe(parentPipe);

  // create a child fork and perform ping pong
  int pid = fork();
  if (pid == 0){ // child
    // fprintf(1, "%d : child Started.. \n", getpid());

    uint8 input;
    read(childPipe[0], &input, 1);

    fprintf(1, "%d: received ping %d \n", getpid(), input);

    uint8 output = 2; 
    write(parentPipe[1], &output, 1);

    exit(0);
  }
  else if (pid > 0 ){ // parent
    // fprintf(1, "%d: Parent continues.. \n", getpid());
    uint8 output = 1; 
    write(childPipe[1], &output, 1);

    uint8 input;
    read(parentPipe[0], &input, 1);
    fprintf(1, "%d: received pong %d \n", getpid(), input);

    wait((int*)0);
  } else {
    fprintf(2, "%d: Error Forking \n", getpid());
  }

  exit(0);
}
