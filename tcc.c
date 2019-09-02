#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// value stands for token type
typedef enum {
  TK_RESERVED,
  TK_NUM,
  TK_EOF,
} TokenKind;

// token type
typedef struct Token Token;
struct Token {
  TokenKind kind;      // token type
  Token *next;         // next input token
  int val;             // num val in case ty is TK_NUM
  char *str;           // token char array (for error message)
};

// input program
char *user_input;

// current token
Token *token;

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
void error_at(char * loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, ""); // output 'pos' num of space
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// Read token and return true if expected symbol.
// Otherwise return false.
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) {
    return false;
  }
  token = token->next;
  return true;
}

// Read token if the token is expected symbol.
// Otherwise report error.
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) {
    error_at(token->str, "Not '%c'", op);
  }
  token = token->next;
}

// Read token and return the number if the token is number.
// Otherwise report error.
int expect_number() {
  if (token->kind != TK_NUM) {
    error_at(token->str, "Not a number");
  }
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

// Create new token and connect with cur
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// tokenize and store strings pointed by user_input into tokens
Token *tokenize() {
  char *p = user_input;
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while(*p) {
    // skip space
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(p, "expected a number");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
      error("%s: Invalid argument num!\n", argv[0]);
    return 1;
  }

  // tokenize
  user_input = argv[1];
  token = tokenize();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // First at expr must be a number.
  // Check it and print out first mov operation
  printf("  mov rax, %d\n", expect_number());

  while (!at_eof()) {
    if (consume('+')) {
      printf("  add rax, %d\n", expect_number());
      continue;
    }

    expect('-');
    printf("  sub rax, %d\n", expect_number());
  }

  printf("  ret\n");
  return 0;
}
