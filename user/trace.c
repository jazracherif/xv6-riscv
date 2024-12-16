#include "kernel/types.h"
#include "user.h"
#include "kernel/syscall.h"
int
main(void)
{
  trace(1 << SYS_sleep);
  sleep(5);

}
