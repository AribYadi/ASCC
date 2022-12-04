#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "globals.h"

void exprPrint(Expr *expr) {
  const char *indent = getPrettyIndent();

  printf("Expr {\n%s  type: '", indent);
  switch (expr->type) {
    case EXPR_INT: printf("EXPR_INT',\n%s  value: '%zu'", indent, expr->value.intv); break;
    case EXPR_STR: printf("EXPR_STR',\n%s  value: '%.*s'", indent, expr->value.str.len, expr->value.str.str); break;
    case EXPR_CHAR: printf("EXPR_CHAR',\n%s  value: '%c'", indent, expr->value.charv); break;
    case EXPR_UNARY: {
      ++indentSize;
      char *pos;
      switch (expr->value.unary.position) {
        case POS_PREFIX: pos = "POS_PREFIX"; break;
        case POS_POSTFIX: pos = "POS_POSTFIX"; break;
      }
      printf("EXPR_UNARY',\n%s  position: '%s',\n%s  opr: ", indent, pos, indent);
      exprPrint(expr->value.unary.opr);
      printf(",\n%s  op: ", indent);
      tokenPrint(&expr->value.unary.op);
      --indentSize;
      break;
    }
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
    case TT_STR: {
      expr.type = EXPR_STR;
      expr.value.str.str = t.lexeme + 1;
      expr.value.str.len = t.lexemeLen - 1;
      break;
    }
    case TT_CHAR: {
      expr.type = EXPR_CHAR;
      expr.value.charv = t.lexeme[1];
      break;
    }
    case TT_PLUS_PLUS:
    case TT_MINUS_MINUS:
    case TT_PLUS:
    case TT_MINUS:
    case TT_BANG:
    case TT_TILDA:
    case TT_STAR:
    case TT_AMPER: {
      int rbp = tokenPrefixBp(&t);
      Expr rhs;
      char *err = parseExpr(lexer, &rhs, rbp);
      if (err) return err;
      expr.type = EXPR_UNARY;
      expr.value.unary.position = POS_PREFIX;
      expr.value.unary.opr = malloc(sizeof(rhs));
      *(Expr *)expr.value.unary.opr = rhs;
      expr.value.unary.op = t;
      break;
    }
    case TT_LPAREN: {
      char *err = parseExpr(lexer, &expr, bp);
      if (err) return err;
      Token t = lexerNext(lexer);
      if (t.type != TT_RPAREN) {
        char *err = malloc(512 * sizeof(char));
        snprintf(err, 512, "Expected 'TT_RPAREN' instead found '%s'!", tokenType(&t));
        return err;
      }
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
      case TT_COMMA:
      case TT_EQ:
      case TT_PLUS_EQ:
      case TT_MINUS_EQ:
      case TT_STAR_EQ:
      case TT_SLASH_EQ:
      case TT_PERCENT_EQ:
      case TT_LESS_LESS_EQ:
      case TT_GREATER_GREATER_EQ:
      case TT_AMPER_EQ:
      case TT_CARAT_EQ:
      case TT_PIPE_EQ:
      case TT_PIPE_PIPE:
      case TT_AMPER_AMPER:
      case TT_PIPE:
      case TT_CARAT:
      case TT_AMPER:
      case TT_EQ_EQ:
      case TT_BANG_EQ:
      case TT_LESS:
      case TT_LESS_EQ:
      case TT_GREATER:
      case TT_GREATER_EQ:
      case TT_LESS_LESS:
      case TT_GREATER_GREATER:
      case TT_PLUS:
      case TT_MINUS:
      case TT_STAR:
      case TT_SLASH:
      case TT_PERCENT:
      case TT_PLUS_PLUS:
      case TT_MINUS_MINUS:
        break;
      case TT_EOF:
      case TT_RPAREN:
        goto end;
        break;
      default: {
        char *err = malloc(512 * sizeof(char));
        snprintf(err, 512, "'%s' is not an operator!", tokenType(&t));
        return err;
      }
    }

    int lbp = tokenPostfixBp(&op);
    if (lbp != -1) {
      lexerNext(lexer);

      Expr lhs = expr;

      expr.type = EXPR_UNARY;
      expr.value.unary.position = POS_POSTFIX;
      expr.value.unary.opr = malloc(sizeof(Expr));
      *(Expr *)expr.value.unary.opr = lhs;
      expr.value.unary.op = op;

      continue;
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
