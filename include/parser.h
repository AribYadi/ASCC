#ifndef ASCC_PARSER_H
#define ASCC_PARSER_H

#include "lexer.h"

typedef enum {
  EXPR_INT,
  EXPR_STR,
  EXPR_CHAR,
  EXPR_BINARY,
} ExprType;

typedef struct {
  char *str;
  int len;
} ExprStr;

typedef struct {
  void *lhs;
  void *rhs;
  Token op;
} ExprBinary;

typedef union {
  size_t intv;
  char charv;
  ExprStr str;
  ExprBinary binary;
} ExprValue;

typedef struct {
  ExprType type;
  ExprValue value;
} Expr;

void exprPrint(Expr *expr);

typedef struct {
  Expr *raw;
  size_t len;
  size_t cap;
} ExprVec;

ExprVec exprVecNew();
void exprVecPush(ExprVec *exprVec, Expr value);
void exprVecPrint(ExprVec *exprVec);
Expr exprVecPop(ExprVec *exprVec);
void exprVecFree(ExprVec *exprVec);

ExprVec parse(char *src);

#endif
