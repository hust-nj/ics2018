#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];
static char *p;
// generate number in [lower, upper]
int gen_rand_num(unsigned lower, unsigned upper)
{
  return (rand() % (upper-lower+1)) + lower;
}


void dfs(int dep)
{
  if (dep == 0)
  {
    int num = gen_rand_num(0, 30);
    sprintf(p, "%d", num);
    while(*++p);
    return;
  }
  else 
  {
    int par = gen_rand_num(0,4);
    if (!par) *p++ = '(';
    int excl = gen_rand_num(0,3);
    if (!excl) *p++ = '!';
    dfs(dep-1);
    if (!par) *p++ = ')'; // left
    int op = gen_rand_num(0,7);
    switch (op)
    {
    case 0:
      *p++ = '+';
      break;
    
    case 1:
      *p++ = '-';
      break;

    case 2:
      *p++ = '*';
      break;

    case 3:
      *p++ = '/';
      break;

    case 4:
      *p++ = '&';
      *p++ = '&';
      break;
    
    case 5:
      *p++ = '|';
      *p++ = '|';
      break;

    case 6:
      *p++ = '=';
      *p++ = '=';
      break;

    case 7:
      *p++ = '!';
      *p++ = '=';

    default:
      break;
    }

    par = gen_rand_num(0,4);
    excl = gen_rand_num(0,3);
    if (!par) *p++ = '(';
    if (!excl) *p++ = '!';
    dfs(dep-1);
    if (!par) *p++ = ')'; // right
    return;
  }
}

static inline void gen_rand_expr() {
  memset(buf, 0, sizeof(buf));
  p = buf;
  int dep = gen_rand_num(1, 9);
  dfs(dep); // dfs parser tree 
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc .code.c -Werror -o .expr");
    if (ret != 0)
    {
      i--;
      continue;
    }

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    if(fscanf(fp, "%d", &result) != 1)
      assert(0);
    pclose(fp);
    printf("%u %s\n", result, buf);
  }
  return 0;
}
