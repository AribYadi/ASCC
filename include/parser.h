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
} String;

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
  String str;
  char charv;
  String ident;
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
  TYPE_INT,
  TYPE_CHAR,
  TYPE_FLOAT,
  TYPE_DOUBLE,
  TYPE_VOID,
  TYPE_IDENT,
  TYPE_POINTER,
} TypeType;

typedef union {
  String ident;
  void *pointType;
} TypeValue;

typedef struct {
  TypeType type;
  TypeValue value;
} Type;

void typePrint(Type *type);
void typeFree(Type type);

typedef enum {
  STMT_EXPR,
  STMT_VARIABLE_DECL,
  STMT_FUNCTION,
  STMT_LABEL,
  STMT_BLOCK,
} StmtType;

typedef struct {
  Type type;
  char *name;
  int nameLen;
  Expr *value;
} StmtVariableDecl;

typedef struct {
  char *name;
  int nameLen;
  Type returnType;
  Type *paramsType;
  Token *paramsName;
  int paramsCount;
  int paramsCap;
  void *body;
} StmtFunction;

typedef union {
  Expr expr;
  StmtVariableDecl varDecl;
  StmtFunction func;
  Token label;
  void *block;
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
