#include "./tcc.h"

// input program
char *user_input;

// current token
Token *token;

void dump() {
  Token *cur = token;
  while(cur) {
    switch(cur->kind) {
    case TK_NUM:
      printf("num: %d\n", cur->val);
      break;
    case TK_IDENT:
      printf("idt: %.1s\n", cur->str);
      break;
    case TK_EOF:
      printf("eof\n");
      break;
    default:
      printf("rsv: %.1s\n", cur->str);
      break;
    }
    cur = cur->next;
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
      error("%s: Invalid argument num!\n", argv[0]);
    return 1;
  }

  // tokenize
  user_input = argv[1];
  token = tokenize();
  //dump();

  // parse
  program();

  // generate code tracing AST
  codegen();

  return 0;
}
