#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp()
{
  WP *node = free_->next; // get a node to use
  if(node == NULL)
    assert(0);
  node->next = head->next; 
  head = node; // add the node to busy list
  free_->next = node->next; // delete the node from free_ list
  return node;
}

void free_wp(WP *wp)
{
  
}


void free_wp(WP *wp);


