#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "globals.h"

#define ERR(str, ...)                                                              \
  do {                                                                             \
    char *err = malloc(1024 * sizeof(char));                                       \
    snprintf(err, 1024, str, ##__VA_ARGS__);                                       \
    return err;                                                                    \
  } while (0);

void exprPrint(Expr *expr) {
  const char *indent = getPrettyIndent();
  ++indentSize;
  const char *indentx = getPrettyIndent();
  --indentSize;

  printf("Expr {\n%stype: '", indentx);
  switch (expr->type) {
    case EXPR_INT: printf("EXPR_INT',\n%svalue: '%zu'", indentx, expr->value.intv); break;
    case EXPR_STR: printf("EXPR_STR',\n%svalue: '%.*s'", indentx, expr->value.str.len, expr->value.str.str); break;
    case EXPR_CHAR: printf("EXPR_CHAR',\n%svalue: '%c'", indentx, expr->value.charv); break;
    case EXPR_IDENT: printf("EXPR_IDENT',\n%svalue: '%.*s'", indentx, expr->value.ident.len, expr->value.ident.str); break;
    case EXPR_UNARY: {
      ++indentSize;
      char *pos;
      switch (expr->value.unary.position) {
        case POS_PREFIX: pos = "POS_PREFIX"; break;
        case POS_POSTFIX: pos = "POS_POSTFIX"; break;
      }
      printf("EXPR_UNARY',\n%sposition: '%s',\n%sopr: ", indentx, pos, indentx);
      exprPrint(expr->value.unary.opr);
      printf(",\n%sop: ", indentx);
      tokenPrint(&expr->value.unary.op);
      --indentSize;
      break;
    }
    case EXPR_BINARY: {
      ++indentSize;
      printf("EXPR_BINARY',\n%slhs: ", indentx);
      exprPrint(expr->value.binary.lhs);
      printf(",\n%srhs: ", indentx);
      exprPrint(expr->value.binary.rhs);
      printf(",\n%sop: ", indentx);
      tokenPrint(&expr->value.binary.op);
      --indentSize;
      break;
    }
    case EXPR_CALL: {
      ++indentSize;
      printf("EXPR_CALL',\n%scallee: ", indentx);
      exprPrint(expr->value.call.callee);
      printf(",\n%sparams: ", indentx);
      exprVecPrint((ExprVec *)expr->value.call.params);
      --indentSize;
      break;
    }
  }
  printf("\n%s}", indent);
}

void exprFree(Expr expr) {
  switch (expr.type) {
    case EXPR_INT:
    case EXPR_STR:
    case EXPR_CHAR:
    case EXPR_IDENT:
      break;
    case EXPR_UNARY: {
      exprFree(*(Expr *)expr.value.unary.opr);
      free(expr.value.unary.opr);
      break;
    }
    case EXPR_BINARY: {
      exprFree(*(Expr *)expr.value.binary.lhs);
      free(expr.value.binary.lhs);
      exprFree(*(Expr *)expr.value.binary.rhs);
      free(expr.value.binary.rhs);
      break;
    }
    case EXPR_CALL: {
      exprFree(*(Expr *)expr.value.call.callee);
      free(expr.value.call.callee);
      exprVecFree(*(ExprVec *)expr.value.call.params);
      free(expr.value.call.params);
      break;
    }
  }
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
  ++indentSize;
  const char *indentx = getPrettyIndent();
  --indentSize;

  if (exprVec->len > 0) {
    printf("ExprVec [\n");
    ++indentSize;
    for (int i = 0; i < exprVec->len; ++i) {
      printf("%s", indentx);
      exprPrint(exprVec->raw + i);
      printf(",\n");
    }
    --indentSize;
    printf("%s]", indent);
  } else printf("ExprVec []");
}

Expr exprVecPop(ExprVec *exprVec) {
  return exprVec->raw[--exprVec->len];
}

void exprVecFree(ExprVec exprVec) {
  while (exprVec.len > 0) {
    Expr expr = exprVecPop(&exprVec);
    exprFree(expr);
  }
  free(exprVec.raw);
}

void typePrint(Type *type) {
  switch (type->type) {
    case TYPE_INT: printf("int"); break;
    case TYPE_CHAR: printf("char"); break;
    case TYPE_FLOAT: printf("float"); break;
    case TYPE_DOUBLE: printf("double"); break;
    case TYPE_VOID: printf("void"); break;
    case TYPE_IDENT: printf("%.*s", type->value.ident.len, type->value.ident.str); break;
    case TYPE_POINTER: {
      typePrint(type->value.pointType);
      printf("*");
      break;
    }
  }
}

void typeFree(Type type) {
  if (type.type == TYPE_POINTER) {
    typeFree(*(Type *)type.value.pointType);
    free(type.value.pointType);
  }
}

void stmtPrint(Stmt *stmt) {
  const char *indent = getPrettyIndent();
  ++indentSize;
  const char *indentx = getPrettyIndent();
  --indentSize;

  printf("Stmt {\n%stype: '", indentx);
  switch (stmt->type) {
    case STMT_EXPR: {
      ++indentSize;
      printf("STMT_EXPR',\n%svalue: ", indentx);
      exprPrint(&stmt->value.expr);
      --indentSize;
      break;
    }
    case STMT_VARIABLE_DECL: {
      ++indentSize;
      printf("STMT_VARIABLE_DECL',\n%svarType: ", indentx);
      typePrint(&stmt->value.varDecl.type);
      printf(",\n%sname: '%.*s'", indentx, stmt->value.varDecl.nameLen, stmt->value.varDecl.name);
      if (stmt->value.varDecl.value) {
        printf(",\n%svalue: ", indentx);
        exprPrint(stmt->value.varDecl.value);
      }
      --indentSize;
      break;
    }
    case STMT_FUNCTION_DECL: {
      ++indentSize;
      printf("STMT_VARIABLE_DECL',\n%sreturnType: ", indentx);
      typePrint(&stmt->value.funcDecl.returnType);
      printf(",\n%sname: '%.*s',\n%sparams: [", indentx, stmt->value.funcDecl.nameLen, stmt->value.funcDecl.name, indentx);

      ++indentSize;
      const char *indentxx = getPrettyIndent();
      ++indentSize;
      const char *indentxxx = getPrettyIndent();
      for (int i = 0; i < stmt->value.funcDecl.paramsCount; ++i) {
        printf("\n%s{", indentxx);
        printf("\n%stype: ", indentxxx);
        typePrint(stmt->value.funcDecl.paramsType + i);
        printf("\n%sname: ", indentxxx);
        tokenPrint(stmt->value.funcDecl.paramsName + i);
        printf("\n%s},", indentxx);
      }
      indentSize -= 2;
      if (stmt->value.funcDecl.paramsCount > 0) printf("\n%s", indentx);
      printf("]");
      --indentSize;
      break;
    }
  }

  printf("\n%s}", indent);
}

void stmtFree(Stmt stmt) {
  switch (stmt.type) {
    case STMT_EXPR: exprFree(stmt.value.expr); break;
    case STMT_VARIABLE_DECL: {
      typeFree(stmt.value.varDecl.type);
      if (stmt.value.varDecl.value) exprFree(*stmt.value.varDecl.value);
      break;
    }
    case STMT_FUNCTION_DECL: {
      for (int i = 0; i < stmt.value.funcDecl.paramsCount; ++i) typeFree(stmt.value.funcDecl.paramsType[i]);
      free(stmt.value.funcDecl.paramsType);
      free(stmt.value.funcDecl.paramsName);
      break;
    }
  }
}

StmtVec stmtVecNew() {
  StmtVec stmtVec = {.raw = malloc(4 * sizeof(Stmt)), .len = 0, .cap = 4};
  return stmtVec;
}

void stmtVecPush(StmtVec *stmtVec, Stmt value) {
  if (stmtVec->cap <= stmtVec->len) {
    stmtVec->cap *= 2;
    stmtVec->raw = realloc(stmtVec->raw, stmtVec->cap * sizeof(Stmt));
    if (!stmtVec->raw) exit(1);
  }
  stmtVec->raw[stmtVec->len++] = value;
}

void stmtVecPrint(StmtVec *stmtVec) {
  const char *indent = getPrettyIndent();
  ++indentSize;
  const char *indentx = getPrettyIndent();
  --indentSize;

  if (stmtVec->len > 0) {
    printf("StmtVec [\n");
    ++indentSize;
    for (int i = 0; i < stmtVec->len; ++i) {
      printf("%s", indentx);
      stmtPrint(stmtVec->raw + i);
      printf(",\n");
    }
    --indentSize;
    printf("%s]", indent);
  } else printf("StmtVec []");
}

Stmt stmtVecPop(StmtVec *stmtVec) {
  return stmtVec->raw[--stmtVec->len];
}

void stmtVecFree(StmtVec stmtVec) {
  while (stmtVec.len > 0) {
    Stmt stmt = stmtVecPop(&stmtVec);
    stmtFree(stmt);
  }
  free(stmtVec.raw);
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
    case TT_IDENT: {
      expr.type = EXPR_IDENT;
      expr.value.ident.str = t.lexeme;
      expr.value.ident.len = t.lexemeLen;
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
        exprFree(expr);
        ERR("Expected 'TT_RPAREN' instead found '%s'!", tokenType(&t));
      }
      break;
    }
    default: ERR("No expression starts with '%s'!", tokenType(&t));
  }

  while (1) {
    Token op = lexerPeek(lexer);
    switch (op.type) {
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
      case TT_LPAREN:
        break;
      case TT_EOF:
      case TT_RPAREN:
      case TT_COMMA:
      case TT_SEMICOLON:
        goto end;
        break;
      default: {
        exprFree(expr);
        ERR("'%s' is not an operator!", tokenType(&t));
      }
    }

    if (op.type == TT_LPAREN) {
      lexerNext(lexer);

      Expr callee = expr;
      ExprVec params = exprVecNew();

      Token t = lexerPeek(lexer);
      while (t.type != TT_RPAREN && t.type != TT_EOF) {
        Expr param;
        char *err = parseExpr(lexer, &param, 0);
        if (err) {
          exprFree(expr);
          exprVecFree(params);
          return err;
        }

        exprVecPush(&params, param);

        t = lexerPeek(lexer);
        if (t.type != TT_COMMA) break;
        lexerNext(lexer);
      }
      t = lexerNext(lexer);
      if (t.type != TT_RPAREN) {
        exprFree(expr);
        exprVecFree(params);
        ERR("Expected 'TT_RPAREN' instead found '%s'!", tokenType(&t));
      }

      expr.type = EXPR_CALL;
      expr.value.call.callee = malloc(sizeof(Expr));
      *(Expr *)expr.value.call.callee = callee;
      expr.value.call.params = malloc(sizeof(ExprVec));
      *(ExprVec *)expr.value.call.params = params;

      continue;
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
    if (err) {
      exprFree(expr);
      return err;
    }

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

char *parseType(Lexer *lexer, Type *buf) {
  Type type;

  switch (lexerPeek(lexer).type) {
    case TT_INT_TYPE: {
      lexerNext(lexer);
      type.type = TYPE_INT;
      break;
    }
    case TT_CHAR_TYPE: {
      lexerNext(lexer);
      type.type = TYPE_CHAR;
      break;
    }
    case TT_FLOAT_TYPE: {
      lexerNext(lexer);
      type.type = TYPE_FLOAT;
      break;
    }
    case TT_DOUBLE_TYPE: {
      lexerNext(lexer);
      type.type = TYPE_DOUBLE;
      break;
    }
    case TT_VOID_TYPE: {
      lexerNext(lexer);
      type.type = TYPE_VOID;
      break;
    }
    case TT_IDENT: {
      Token t = lexerNext(lexer);
      type.type = TYPE_IDENT;
      type.value.ident.str = t.lexeme;
      type.value.ident.len = t.lexemeLen;
    }
    default: {
      Token t = lexerNext(lexer);
      ERR("Expected type but found '%s'!", tokenType(&t));
    }
  }

  while (lexerPeek(lexer).type == TT_STAR) {
    lexerNext(lexer);
    Type pointType = type;
    type.type = TYPE_POINTER;
    type.value.pointType = malloc(sizeof(Type));
    *(Type *)type.value.pointType = pointType;
  }

  *buf = type;
  return NULL;
}

char *parseStmt(Lexer *lexer, Stmt *buf) {
  Stmt stmt;

  Type type;
  char *err = parseType(lexer, &type);
  int foundType = err == NULL;
  if (err) free(err);
  if (foundType && lexerPeek(lexer).type == TT_IDENT) {
    Token t = lexerNext(lexer);
    switch (lexerPeek(lexer).type) {
      case TT_SEMICOLON: {
        stmt.type = STMT_VARIABLE_DECL;
        stmt.value.varDecl.type = type;
        stmt.value.varDecl.name = t.lexeme;
        stmt.value.varDecl.nameLen = t.lexemeLen;
        stmt.value.varDecl.value = NULL;

        break;
      }
      case TT_EQ: {
        stmt.type = STMT_VARIABLE_DECL;
        stmt.value.varDecl.type = type;
        stmt.value.varDecl.name = t.lexeme;
        stmt.value.varDecl.nameLen = t.lexemeLen;
        lexerNext(lexer);

        Expr value;
        char *err = parseExpr(lexer, &value, 0);
        if (err) {
          stmtFree(stmt);
          return err;
        }
        stmt.value.varDecl.value = malloc(sizeof(Expr));
        *stmt.value.varDecl.value = value;

        break;
      }
      case TT_LPAREN: {
        stmt.type = STMT_FUNCTION_DECL;
        stmt.value.funcDecl.name = t.lexeme;
        stmt.value.funcDecl.nameLen = t.lexemeLen;
        stmt.value.funcDecl.returnType = type;
        stmt.value.funcDecl.paramsType = malloc(4 * sizeof(Type));
        stmt.value.funcDecl.paramsName = malloc(4 * sizeof(Token));
        stmt.value.funcDecl.paramsCount = 0;
        stmt.value.funcDecl.paramsCap = 8;
        lexerNext(lexer);

        while (lexerPeek(lexer).type != TT_RPAREN && lexerPeek(lexer).type != TT_EOF) {
          Type type;
          char *err = parseType(lexer, &type);
          if (err) {
            stmtFree(stmt);
            return err;
          }
          Token name = lexerNext(lexer);
          if (name.type != TT_IDENT) {
            typeFree(type);
            stmtFree(stmt);
            ERR("Expected 'TT_IDENT' instead found '%s'!", tokenType(&name));
          }

          if (stmt.value.funcDecl.paramsCap <= stmt.value.funcDecl.paramsCount) {
            stmt.value.funcDecl.paramsCap *= 2;
            stmt.value.funcDecl.paramsType = realloc(stmt.value.funcDecl.paramsType, stmt.value.funcDecl.paramsCap * sizeof(Type));
            stmt.value.funcDecl.paramsName = realloc(stmt.value.funcDecl.paramsName, stmt.value.funcDecl.paramsCap * sizeof(Token));
            if (!stmt.value.funcDecl.paramsType || !stmt.value.funcDecl.paramsName) exit(1);
          }
          stmt.value.funcDecl.paramsType[stmt.value.funcDecl.paramsCount] = type;
          stmt.value.funcDecl.paramsName[stmt.value.funcDecl.paramsCount++] = name;

          if (lexerPeek(lexer).type != TT_COMMA) break;
          lexerNext(lexer);
        }
        Token t = lexerNext(lexer);
        if (t.type != TT_RPAREN) {
          typeFree(type);
          stmtFree(stmt);
          ERR("Expected 'TT_RPAREN' instead found '%s'!", tokenType(&t));
        }

        break;
      }
      default: {
        typeFree(type);
        stmtFree(stmt);
        ERR("Expected 'TT_SEMICOLON' or 'TT_EQ' instead found '%s'!", tokenType(&t));
      }
    }
  } else if (foundType) {
    typeFree(type);
  } else {
    stmt.type = STMT_EXPR;
    char *err = parseExpr(lexer, &stmt.value.expr, 0);
    if (err) return err;
  }

  Token t = lexerNext(lexer);
  if (t.type != TT_SEMICOLON) {
    stmtFree(stmt);
    ERR("Expected 'TT_SEMICOLON' instead found '%s'!", tokenType(&t));
  }

  *buf = stmt;
  return NULL;
}

StmtVec parse(char *src) {
  StmtVec stmts = stmtVecNew();
  Lexer lexer = lexerNew(src);

  while (lexerPeek(&lexer).type != TT_EOF) {
    Stmt stmt;
    char *err = parseStmt(&lexer, &stmt);
    if (!err) stmtVecPush(&stmts, stmt);
    else {
      fprintf(stderr, "%s\n", err);
      free(err);
      exit(1);
    }
  }

  return stmts;
}
