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
  int len;             // token char length
};

// input program
extern char *user_input;
// current token
extern Token *token;

void error(char *fmt, ...);
void error_at(char * loc, char *fmt, ...);
Token *tokenize();

// Kind of Node for AST
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // integer
  ND_LT, // less than <
  ND_LE, // less than or equal <=
  ND_EQ, // equal ==
  ND_NE, // not equal !=
} NodeKind;

typedef struct Node Node;
struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
};

// parse.c
Node *program();

// codegen.c
void codegen(Node *node);
