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
    wp_pool[i].busy = false;
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

void print_wp()
{
  printf("Num\tWhat\n");
  for (WP *p = head; p; p = p->next)
  {
    printf("%d\t%s\n", p->NO, p->expr);
  }
}

WP* new_wp()
{
  WP *node = free_; // get a node to use
  if(node == NULL)
    assert(0);
  free_ = node->next; // delete the node from free_ list
  node->next = head; 
  head = node; 
  node->busy = true;// add the node to busy list
  return node;
}

void free_wp(int id) // move block id to free_ list
{
  if(wp_pool[id].busy == false)
  {
    printf("cannot find watch point id.");
    return;
  }
  
  WP *p, *plast;
  if (head->NO == id)
  {
    p = head;
    head = head->next;
    p->next = free_;
    p->busy = false;
    free_ = p;
    return;
  }

  for (p = head->next, plast = head; p; p = p->next)
  {
    if(p->NO == id)
    {
      plast->next = p->next; // delete node from busy list
      p->next = free_;
      p->busy = false;
      free_ = p; // add node to free_ list
      break;
    }
    plast = p;
  }
}


