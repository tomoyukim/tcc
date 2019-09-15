#include "./tcc.h"

// Read token and return true if expected symbol.
// Otherwise return false.
bool consume(char *op) {
  if (token->kind != TK_RESERVED ||
      strlen(op) != token->len ||
      memcmp(token->str, op, token->len)) {
    return false;
  }
  token = token->next;
  return true;
}

Token *consume_ident() {
  if (token->kind != TK_IDENT) {
      return NULL;
  }
  Token *t = token;
  token = token->next;
  return t;
}

// Read token if the token is expected symbol.
// Otherwise report error.
void expect(char *op) {
  if (token->kind != TK_RESERVED ||
      strlen(op) != token->len ||
      memcmp(token->str, op, token->len)) {
    error_at(token->str, "Not '%s'", op);
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

Node *new_node(NodeKind kind) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  return node;
}

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = new_node(kind);
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_num(int val) {
  Node *node = new_node(ND_NUM);
  node->val = val;
  return node;
}

Node *new_var(int offset) {
  Node *node = new_node(ND_LVAR);
  node->offset = offset;
  return node;
}

Node *expr();

// primary = "(" expr ")" | num
Node *primary() {
  // Must be "(" expr ")" if next token is "("
  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  }
  Token *t = consume_ident();
  if (t) {
    return new_var((t->str[0] - 'a' + 1) * 8);
  }
  // otherwise, expect a number;
  return new_num(expect_number());
}

// unary = ("+" | "-")? primary
Node *unary() {
    if (consume("+")) {
        return unary();
    } else if (consume("-")) {
        return new_binary(ND_SUB, new_num(0), unary());
    } else {
        return primary();
    }
}

// mul = primary ("*" primary | "/" primary)*
Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume("*")) {
      node = new_binary(ND_MUL, node, unary());
    } else if (consume("/")) {
      node = new_binary(ND_DIV, node, unary());
    } else {
      return node;
    }
  }
}

// add = mul ("+" mul | "-" mul)*
Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume("+")) {
      node = new_binary(ND_ADD, node, mul());
    } else if (consume("-")) {
      node = new_binary(ND_SUB, node, mul());
    } else {
      return node;
    }
  }
}

// relational = add ("<" add | "<=" add | ">" add | "=>" add)*
Node *relational() {
  Node *node =  add();

  for(;;) {
    if (consume("<")) { // less than
      node = new_binary(ND_LT, node, add());
    } else if (consume("<=")) {
      node = new_binary(ND_LE, node, add());
    } else if (consume(">")) {
      node = new_binary(ND_LT, add(), node);
    } else if (consume(">=")) {
      node = new_binary(ND_LE, add(), node);
    } else {
      return node;
    }
  }
}

// equality = relational ("=" relational | "!=" relational)*
Node *equality() {
  Node *node = relational();

  for(;;) {
    if (consume("==")) { // less than
      node = new_binary(ND_EQ, node, relational());
    } else if (consume("!=")) {
      node = new_binary(ND_NE, node, relational());
    } else {
      return node;
    }
  }
}

// assign = equality ("=" assign)?
Node *assign() {
  Node *node = equality();
  if (consume("=")) {
    node = new_binary(ND_ASSIGN, node, assign());
  }
  return node;
}

// expr = assign
Node *expr() {
  return assign();
}

Node *stmt() {
  Node *node = expr();
  expect(";");
  return node;
}

void program() {
  int i = 0;
  while (!at_eof()) {
    code[i++] = stmt();
  }
  code[i] = NULL;
}
