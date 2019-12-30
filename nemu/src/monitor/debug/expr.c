#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum
{
  TK_NOTYPE = 256,
  TK_HEX,
  TK_DEC,
  TK_REG,
  TK_EQ,
  TK_POS,
  TK_NEG

  /* TODO: Add more token types */

};

static struct rule
{
  char *regex;
  int token_type;
} rules[] = {

    /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

    {" +", TK_NOTYPE},                // spaces
    {"\\+", '+'},                     // plus
    {"\\-", '-'},                     // minus
    {"\\*", '*'},                     // mult
    {"/", '/'},                       // divide
    {"\\(", '('},                     // left par
    {"\\)", ')'},                     // right par
    {"0[xX][0-9a-fA-F]+", TK_HEX},    // hexadecimal
    {"[0-9][0-9]*", TK_DEC},          // decimal
    {"\\$[a-zA-Z][a-zA-Z]+", TK_REG}, // register
    {"==", TK_EQ}                     // equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i++)
  {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0)
    {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token
{
  int type;
  char str[32];
  int sign;
} Token;

Token tokens[1024];
int nr_token;

static bool make_token(char *e)
{
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0')
  {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++)
    {
      printf("checkpoint 1");
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
      {
        printf("checkpoint 2");
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //     i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        if (rules[i].token_type == TK_NOTYPE)
          break; // match the blank string, go next

        switch (rules[i].token_type)
        {
        case TK_DEC:
        case TK_HEX:
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          tokens[nr_token].str[substr_len] = '\0';
          tokens[nr_token].sign = 1;
          break;

        case TK_REG:
          strncpy(tokens[nr_token].str, substr_start + 1, substr_len - 1);
          tokens[nr_token].str[substr_len - 1] = '\0';
          tokens[nr_token].sign = 1;
          break;

        default:;
          // printf("%d\n", rules[i].token_type);
          // TODO();
        }
        tokens[nr_token].type = rules[i].token_type;
        ++nr_token; // successfully match the token
        break;
      }
    }

    if (i == NR_REGEX)
    {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

uint32_t reg2u(char *reg, bool *success)
{
  for (char *pstr = reg; *pstr; ++pstr)
  {
    *pstr = tolower(*pstr);
  }

  if (!strcmp(reg, "eip"))
  { // eip case
    return cpu.eip;
  }
  else if (strlen(reg) == 3)
  { // eax... case
    for (int i = R_EAX; i <= R_EDI; ++i)
    {
      if (!strcmp(reg, regsl[i]))
        return reg_l(i);
    }
  }
  else if (*(reg + 1) != 'h' && *(reg + 1) != 'l')
  { // ax... case
    for (int i = R_AX; i <= R_DI; ++i)
    {
      if (!strcmp(reg, regsw[i]))
        return reg_w(i);
    }
  }
  else
  { // al... or ah... case
    for (int i = R_AL; i <= R_BH; ++i)
    {
      if (!strcmp(reg, regsb[i]))
        return reg_b(i);
    }
  }

  return 0; // cannot find the register
}

bool check_parentheses(int p, int q)
{
  if (tokens[p].type == '(' && tokens[q].type == ')')
    return true;
  else
    return false;
}

#define ismd(i) (tokens[i].type == '*' || tokens[i].type == '/')
#define ispm(i) (tokens[i].type == '+' || tokens[i].type == '-')
#define isnum(i) (tokens[i].type == TK_HEX || tokens[i].type == TK_DEC || tokens[i].type == TK_REG || tokens[i].type == ')')

long long eval(int p, int q, bool *success)
{
  if (!*success)
    return 0;
  if (p > q)
  {
    /* Bad expression */
    *success = false;
  }
  else if (p == q)
  {
    /* Single token.
     */
    switch (tokens[p].type)
    {
    case TK_DEC:
      return strtoll(tokens[p].str, NULL, 10) * tokens[p].sign;
      break;

    case TK_HEX:
      return strtoll(tokens[p].str, NULL, 16) * tokens[p].sign;
      break;

    case TK_REG:
      return reg2u(tokens[p].str, success) * tokens[p].sign;
      break;
    default:
      *success = false;
      return 0;
      break;
    }
  }
  else if (check_parentheses(p, q) == true)
  {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1, success);
  }
  else
  {
    /* We should do more things here. */

    while (tokens[p].type == TK_POS || tokens[p].type == TK_NEG)
    {
      if (tokens[p].type == TK_POS)
      {
        p++;
      }
      if (tokens[p].type == TK_NEG)
      {
        if (p < nr_token - 1)
          tokens[p + 1].sign *= -1;
        else
        {
          *success = false;
          return 0;
        }
        p++;
      }
    }

    int i, inpar = 0; // the numbers of par surrounding between p and q
    int op = 0;
    *success = false;
    for (i = p; i < q; ++i)
    {
      if (tokens[i].type == ')')
      {
        inpar--;
        if (inpar < 0)
        {
          // Log("error for inpar < 0");
          *success = false;
          return 0;
        }
      }
      if (tokens[i].type == '(')
      {
        // Log("inpar ++");
        inpar++;
      }

      if (!inpar)
      {
        if (ismd(i) && (op == 0 || !ispm(op)))
        {
          // Log("find mult or div");
          op = i;
          *success = true;
        }
        else if (ispm(i))
        {
          // Log("find plus or minus");
          op = i;
          *success = true;
        }
      }
    }
    // Log("success = %d", *success);
    if (*success == false)
      return 0;

    // Log("operator %c\n", tokens[i].type);

    // now we find the op to split
    long long val1, val2;
    val1 = eval(p, op - 1, success);
    if (!*success)
      return 0;
    val2 = eval(op + 1, q, success);
    if (!*success)
      return 0;

    switch (tokens[op].type)
    {
    case '+':
      return val1 + val2;
    case '-':
      return val1 - val2;
    case '*':
      return val1 * val2;
    case '/':
      if (val2 == 0)
      {
        *success = false;
        printf("Divide zero encounting!\n");
        return 0;
      }
      else
        return val1 / val2;
    default:
      *success = false;
      return 0;
    }
  }
  return 0;
}

uint32_t expr(char *e, bool *success)
{
  if (!make_token(e))
  { // fail
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  if (tokens[0].type == '+')
    tokens[0].type = TK_POS;
  if (tokens[0].type == '-')
    tokens[0].type = TK_NEG;
  for (int i = 1; i < nr_token; ++i)
  {
    if (!isnum(i - 1))
    {
      if (tokens[i].type == '*')
        tokens[i].type = TK_POS;
      if (tokens[i].type == '-')
        tokens[i].type = TK_NEG;
    }
  }

  // Log("nr_token = %d", nr_token);

  *success = true;
  int tmp = eval(0, nr_token - 1, success);
  if (*success)
    return tmp;

  return 0;
}
