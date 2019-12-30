int init_monitor(int, char *[]);
void ui_mainloop(int);


#include "nemu.h"
uint32_t expr(char *e, bool *success);

int main(int argc, char *argv[]) {
  
  /* test expr */
  FILE *fp;
  if(!(fp = fopen("tools/gen-expr/input", "r")))
  {
    printf("cannot open file input\n");
    exit(1);
  }
  unsigned res;
  char str[1024];
  bool success;
  bool pass = true;
  int cnt = 0;
  while (fscanf(fp, "%u%[^\n]", &res, str) == 2)
  {
    unsigned tmp = expr(str, &success);
    pass = pass && success && (tmp == res);
    cnt++;
  }
  fclose(fp);

  if (pass)
    printf("success fully pass all %d test cases\n", cnt);
  

  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
