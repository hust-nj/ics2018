#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

void print_wp();

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */


} WP;

#endif
