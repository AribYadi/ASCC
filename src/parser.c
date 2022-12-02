#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

void exprPrint(Expr *expr) {
  printf("Expr { type: '");
  switch (expr->type) {
    case EXPR_INT: printf("EXPR_INT', value: '%zu'", expr->value.intv); break;
  }
  printf(" }\n");
}

ExprVec exprVecNew() {
  ExprVec exprVec = {.raw = malloc(4 * sizeof(Expr)), .len = 0, .cap = 4};
  return exprVec;
}

void exprVecPush(ExprVec *exprVec, Expr value) {
  if (exprVec->cap <= exprVec->len) {
    exprVec->cap *= 2;
    exprVec->raw = realloc(exprVec->raw, exprVec->cap * sizeof(Expr));
    if (!exprVec->raw) exit(1);
  }
  exprVec->raw[exprVec->len++] = value;
}

void exprVecPrint(ExprVec *exprVec) {
  printf("ExprVec [\n");
  for (int i = 0; i < exprVec->len; ++i) {
    printf("  ");
    exprPrint(exprVec->raw + i);
  }
  printf("]\n");
}

Expr exprVecPop(ExprVec *exprVec) {
  return exprVec->raw[--exprVec->len];
}

void exprVecFree(ExprVec *exprVec) {
  free(exprVec->raw);
}

ExprVec parse(char *src) {
  ExprVec exprs = exprVecNew();
  for (int i = 0; i < 10; ++i) {
    exprVecPush(&exprs, (Expr){.type = EXPR_INT, .value = i});
  }
  return exprs;
}
