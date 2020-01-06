#include "common.h"
#include "syscall.h"
#include "proc.h"
#include "fs.h"

extern void naive_uload(PCB *pcb, const char *filename);

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  uintptr_t ret = -1;
  a[0] = c->GPR1;
  a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_yield:
      ret = 0;
      printf("system yeild\n");
      break;
    case SYS_exit:
      printf("system exit\n");
      _halt(a[1]);
      break;
    case SYS_write:
      ret = fs_write(a[1], (void*)a[2], a[3]);
      // printf("system write:%d\n", ret); // do not use printf to debug!
      break;
    case SYS_brk:
      ret = 0; // always return zero / always succeed
      break;
    case SYS_open:
      ret = fs_open((void *)a[1], a[2], a[3]);
      break;
    case SYS_close:
      ret = fs_close(a[1]);
      break;
    case SYS_read:
      ret = fs_read(a[1], (void *)a[2], a[3]);
      break;
    case SYS_lseek:
  		ret = fs_lseek(a[1], a[2], a[3]);
      break;
    case SYS_execve:
  			naive_uload(NULL, (void *)a[1]);
  			break;

    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  c->GPR1 = ret;
  return NULL;
}
