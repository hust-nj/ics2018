#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  uintptr_t ret = -1;
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_yield:
      ret = 0;
      printf("system yeild\n");
      break;

    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  c->GPR1 = ret;
  return NULL;
}
