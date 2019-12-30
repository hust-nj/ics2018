int init_monitor(int, char *[]);
void ui_mainloop(int);

#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include "nemu.h"
uint32_t expr(char *e, bool *success);

int main(int argc, char *argv[]) {
  
  /* test expr */
   char cwd[1000];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       return 1;
   }


  FILE *fp;
  if(!(fp = fopen("tools/gen-expr/input", "r")))
  {
    
    printf("cannot open file input\n");
    exit(1);
  }
  unsigned res;
  char str[2048];
  bool success;
  bool pass = true;
  int cnt = 0;
  while (fscanf(fp, "%u %[^\n]", &res, str) == 2)
  {
    printf("%u\t%s\n", res, str);
    unsigned tmp = expr(str, &success);
    printf("%d\n", tmp);
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
