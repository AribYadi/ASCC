#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "globals.h"

void exprPrint(Expr *expr) {
  const char *indent = getPrettyIndent();

  printf("Expr {\n%s  type: '", indent);
  switch (expr->type) {
    case EXPR_INT: printf("EXPR_INT',\n%s  value: '%zu'", indent, expr->value.intv); break;
    case EXPR_BINARY: {
      ++indentSize;
      printf("EXPR_BINARY',\n%s  lhs: ", indent);
      exprPrint(expr->value.binary.lhs);
      printf(",\n%s  rhs: ", indent);
      exprPrint(expr->value.binary.rhs);
      printf(",\n%s  op: ", indent);
      tokenPrint(&expr->value.binary.op);
      --indentSize;
      break;
    }
  }
  printf("\n%s}", indent);
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
  const char *indent = getPrettyIndent();

  printf("ExprVec [\n");
  ++indentSize;
  for (int i = 0; i < exprVec->len; ++i) {
    printf("%s  ", indent);
    exprPrint(exprVec->raw + i);
    printf("\n");
  }
  --indentSize;
  printf("]");
}

Expr exprVecPop(ExprVec *exprVec) {
  return exprVec->raw[--exprVec->len];
}

void exprVecFree(ExprVec *exprVec) {
  free(exprVec->raw);
}

char *parseExpr(Lexer *lexer, Expr *buf, int bp) {
  Expr expr;
  Token t = lexerNext(lexer);
  switch (t.type) {
    case TT_INT: {
      expr.type = EXPR_INT;
      expr.value.intv = strtoull(tokenLexeme(&t), NULL, 10);
      break;
    }
    default: {
      char *err = malloc(512 * sizeof(char));
      snprintf(err, 512, "No expression starts with '%s'!", tokenType(&t));
      return err;
    }
  }

  while (1) {
    Token op = lexerPeek(lexer);
    switch (op.type) {
      case TT_PLUS:
      case TT_MINUS:
      case TT_STAR:
      case TT_SLASH:
        break;
      case TT_EOF: goto end; break;
      default: {
        char *err = malloc(512 * sizeof(char));
        snprintf(err, 512, "'%s' is not an operator!", tokenType(&t));
        return err;
      }
    }

    int *op_bp = tokenInfixBp(&op);
    if (!op_bp || op_bp[0] < bp) break;

    lexerNext(lexer);

    Expr rhs;
    char *err = parseExpr(lexer, &rhs, op_bp[1]);
    if (err) return err;

    Expr lhs = expr;

    expr.type = EXPR_BINARY;
    expr.value.binary.lhs = malloc(sizeof(Expr));
    *(Expr *)expr.value.binary.lhs = lhs;
    expr.value.binary.rhs = malloc(sizeof(Expr));
    *(Expr *)expr.value.binary.rhs = rhs;
    expr.value.binary.op = op;
  }

end:
  *buf = expr;
  return NULL;
}

ExprVec parse(char *src) {
  ExprVec exprs = exprVecNew();
  Lexer lexer = lexerNew(src);

  while (lexerPeek(&lexer).type != TT_EOF) {
    Expr expr;
    char *err = parseExpr(&lexer, &expr, 0);
    if (!err) exprVecPush(&exprs, expr);
    else {
      fprintf(stderr, "%s\n", err);
      free(err);
      exit(1);
    }
  }

  return exprs;
}
