#include "./tcc.h"

// input program
char *user_input;

// current token
Token *token;

int main(int argc, char **argv) {
  if (argc != 2) {
      error("%s: Invalid argument num!\n", argv[0]);
    return 1;
  }

  // tokenize
  user_input = argv[1];
  token = tokenize();

  // parse
  Node *node = program();

  // generate code tracing AST
  codegen(node);

  return 0;
}
