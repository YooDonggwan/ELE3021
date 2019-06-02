# Initial process execs /init.
# This code runs in user space.

#include "syscall.h"
#include "traps.h"


# exec(myinit, argv)
.globl start
start:
  pushl $argv
  pushl $myinit
  pushl $0  // where caller pc would be
  movl $SYS_exec, %eax
  int $T_SYSCALL

# for(;;) exit();
exit:
  movl $SYS_exit, %eax
  int $T_SYSCALL
  jmp exit

# char myinit[] = "/myinit\0";
myinit:
  .string "/myinit\0"

# char *argv[] = { myinit, 0 };
.p2align 2
argv:
  .long myinit
  .long 0

