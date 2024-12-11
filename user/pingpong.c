#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "Usage: pingpong\n");
    exit(1);
  }

  #define READ 0
  #define WRITE 1
  // Create 2 pipes
  int pipe1[2]; // child to parent
  int pipe2[2]; // parent to child

  pipe(pipe1);
  pipe(pipe2);

  // create a child fork and perform ping pong
  int pid = fork();
  if (pid == 0){ // child
    // fprintf(1, "%d : child Started.. \n", getpid());

    uint8 input;
    read(pipe1[READ], &input, 1);

    fprintf(1, "%d: received ping %d \n", getpid(), input);

    uint8 output = 2; 
    write(pipe2[WRITE], &output, 1);

    exit(0);
  }
  else if (pid > 0 ){ // parent
    // fprintf(1, "%d: Parent continues.. \n", getpid());
    uint8 output = 1; 
    write(pipe1[WRITE], &output, 1);

    uint8 input;
    read(pipe2[READ], &input, 1);
    fprintf(1, "%d: received pong %d \n", getpid(), input);

    wait((int*)0);
  } else {
    fprintf(2, "%d: Error Forking \n", getpid());
  }

  exit(0);
}
