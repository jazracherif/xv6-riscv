#include "kernel/types.h"
#include "user.h"
#include "kernel/syscall.h"
int
main(void)
{
  trace(1 << SYS_sleep);
  sleep(5);

  //check child inheriting
  int pid = fork();
  if (pid == 0) { //child
     sleep(5);
     exit(0);
  } else if (pid > 0) {
    wait((int*)0);
  }

  int mask = 1 << SYS_sleep | 1 << SYS_getpid;
  trace(mask);
  getpid();
  sleep(5);

}
