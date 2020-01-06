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
      // printf("system write:%d\n", ret);
      break;

    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  c->GPR1 = ret;
  return NULL;
}
