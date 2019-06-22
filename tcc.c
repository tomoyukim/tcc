#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// value stands for token type
enum {
  TK_NUM = 256,
  TK_EOF,
};

// token type
typedef struct {
  int ty;      // token type
  int val;     // num val in case ty is TK_NUM
  char *input; // token char array (for error message)
} Token;

// input program
char *user_input;

// Tokenized result
// Assumes token doesn't come more than 100
Token tokens[100];

// error report func
// same args as printf
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// error point report func
void error_at(char * loc, char *msg) {
  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, ""); // output 'pos' num of space
  fprintf(stderr, "^ %s\n", msg);
  exit(1);
}

// tokenize and store strings pointed by user_input into tokens
void tokenize() {
  char *p = user_input;

  int i = 0;
  while(*p) {
    // skip space
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }

    if (isdigit(*p)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = p;
      tokens[i].val = strtol(p, &p, 10);
      i++;
      continue;
    }

    error_at(p, "cannot tokenize");
  }

  tokens[i].ty = TK_EOF;
  tokens[i].input = p;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid argument num!\n");
    return 1;
  }

  // tokenize
  user_input = argv[1];
  tokenize();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  if (tokens[0].ty != TK_NUM)
    error_at(tokens[0].input, "Not a number");
  printf("  mov rax, %d\n", tokens[0].val);

  int i = 1;
  while (tokens[i].ty != TK_EOF) {
    if (tokens[i].ty == '+') {
      i++;
      if (tokens[i].ty != TK_NUM)
	error_at(tokens[i].input, "Not a number");
      printf("  add rax, %d\n", tokens[i].val);
      i++;
      continue;
    }

    if (tokens[i].ty == '-') {
      i++;
      if (tokens[i].ty != TK_NUM)
	error_at(tokens[i].input, "Not a number");
      printf("  sub rax, %d\n", tokens[i].val);
      i++;
      continue;
    }

    error_at(tokens[i].input, "Unexpected token");
  }
  
  printf("  ret\n");
  return 0;
}
