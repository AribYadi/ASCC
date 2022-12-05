#ifndef ASCC_PARSER_H
#define ASCC_PARSER_H

#include "lexer.h"

typedef enum {
  EXPR_INT,
  EXPR_STR,
  EXPR_CHAR,
  EXPR_IDENT,
  EXPR_UNARY,
  EXPR_BINARY,
  EXPR_CALL,
} ExprType;

typedef struct {
  char *str;
  int len;
} ExprStr;

typedef struct {
  enum {
    POS_PREFIX,
    POS_POSTFIX,
  } position;
  void *opr;
  Token op;
} ExprUnary;

typedef struct {
  void *lhs;
  void *rhs;
  Token op;
} ExprBinary;

typedef struct {
  void *callee;
  void *params;
} ExprCall;

typedef union {
  size_t intv;
  ExprStr str;
  char charv;
  ExprStr ident;
  ExprUnary unary;
  ExprBinary binary;
  ExprCall call;
} ExprValue;

typedef struct {
  ExprType type;
  ExprValue value;
} Expr;

void exprPrint(Expr *expr);
void exprFree(Expr expr);

typedef struct {
  Expr *raw;
  size_t len;
  size_t cap;
} ExprVec;

ExprVec exprVecNew();
void exprVecPush(ExprVec *exprVec, Expr value);
void exprVecPrint(ExprVec *exprVec);
Expr exprVecPop(ExprVec *exprVec);
void exprVecFree(ExprVec exprVec);

typedef enum {
  STMT_EXPR
} StmtType;

typedef union {
  Expr expr;
} StmtValue;

typedef struct {
  StmtType type;
  StmtValue value;
} Stmt;

void stmtPrint(Stmt *stmt);
void stmtFree(Stmt stmt);

typedef struct {
  Stmt *raw;
  size_t len;
  size_t cap;
} StmtVec;

StmtVec stmtVecNew();
void stmtVecPush(StmtVec *stmtVec, Stmt value);
void stmtVecPrint(StmtVec *stmtVec);
Stmt stmtVecPop(StmtVec *stmtVec);
void stmtVecFree(StmtVec stmtVec);

StmtVec parse(char *src);

#endif
