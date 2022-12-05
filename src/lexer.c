#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"
#include "globals.h"

const char *tokenType(Token *t) {
  switch (t->type) {
    case TT_EOF: return "TT_EOF";
    case TT_UNKNOWN: return "TT_UNKNOWN";
    case TT_UNCLOSED_STR: return "TT_UNCLOSED_STR";
    case TT_UNCLOSED_CHAR: return "TT_UNCLOSED_CHAR";
    case TT_INT_TYPE: return "TT_INT_TYPE";
    case TT_CHAR_TYPE: return "TT_CHAR_TYPE";
    case TT_FLOAT_TYPE: return "TT_FLOAT_TYPE";
    case TT_DOUBLE_TYPE: return "TT_DOUBLE_TYPE";
    case TT_LONG: return "TT_LONG";
    case TT_UNSIGNED: return "TT_UNSIGNED";
    case TT_AUTO: return "TT_AUTO";
    case TT_BREAK: return "TT_BREAK";
    case TT_CASE: return "TT_CASE";
    case TT_CONST: return "TT_CONST";
    case TT_CONTINUE: return "TT_CONTINUE";
    case TT_DEFAULT: return "TT_DEFAULT";
    case TT_DO: return "TT_DO";
    case TT_ELSE: return "TT_ELSE";
    case TT_ENUM: return "TT_ENUM";
    case TT_EXTERN: return "TT_EXTERN";
    case TT_FOR: return "TT_FOR";
    case TT_GOTO: return "TT_GOTO";
    case TT_IF: return "TT_IF";
    case TT_REGISTER: return "TT_REGISTER";
    case TT_RETURN: return "TT_RETURN";
    case TT_SHORT: return "TT_SHORT";
    case TT_SIGNED: return "TT_SIGNED";
    case TT_SIZEOF: return "TT_SIZEOF";
    case TT_STATIC: return "TT_STATIC";
    case TT_STRUCT: return "TT_STRUCT";
    case TT_SWITCH: return "TT_SWITCH";
    case TT_TYPEDEF: return "TT_TYPEDEF";
    case TT_UNION: return "TT_UNION";
    case TT_VOID_TYPE: return "TT_VOID_TYPE";
    case TT_VOLATILE: return "TT_VOLATILE";
    case TT_WHILE: return "TT_WHILE";
    case TT_INT: return "TT_INT";
    case TT_STR: return "TT_STR";
    case TT_CHAR: return "TT_CHAR";
    case TT_IDENT: return "TT_IDENT";
    case TT_PLUS: return "TT_PLUS";
    case TT_PLUS_EQ: return "TT_PLUS_EQ";
    case TT_MINUS: return "TT_MINUS";
    case TT_MINUS_EQ: return "TT_MINUS_EQ";
    case TT_STAR: return "TT_STAR";
    case TT_STAR_EQ: return "TT_STAR_EQ";
    case TT_SLASH: return "TT_SLASH";
    case TT_SLASH_EQ: return "TT_SLASH_EQ";
    case TT_LESS: return "TT_LESS";
    case TT_LESS_EQ: return "TT_LESS_EQ";
    case TT_GREATER: return "TT_GREATER";
    case TT_GREATER_EQ: return "TT_GREATER_EQ";
    case TT_EQ: return "TT_EQ";
    case TT_EQ_EQ: return "TT_EQ_EQ";
    case TT_BANG: return "TT_BANG";
    case TT_BANG_EQ: return "TT_BANG_EQ";
    case TT_AMPER_AMPER: return "TT_AMPER_AMPER";
    case TT_PIPE_PIPE: return "TT_PIPE_PIPE";
    case TT_TILDA: return "TT_TILDA";
    case TT_CARAT: return "TT_CARAT";
    case TT_CARAT_EQ: return "TT_CARAT_EQ";
    case TT_PERCENT: return "TT_PERCENT";
    case TT_PERCENT_EQ: return "TT_PERCENT_EQ";
    case TT_LESS_LESS: return "TT_LESS_LESS";
    case TT_LESS_LESS_EQ: return "TT_LESS_LESS_EQ";
    case TT_GREATER_GREATER: return "TT_GREATER_GREATER";
    case TT_GREATER_GREATER_EQ: return "TT_GREATER_GREATER_EQ";
    case TT_AMPER_EQ: return "TT_AMPER_EQ";
    case TT_PIPE_EQ: return "TT_PIPE_EQ";
    case TT_LPAREN: return "TT_LPAREN";
    case TT_RPAREN: return "TT_RPAREN";
    case TT_LBRACE: return "TT_LBRACE";
    case TT_RBRACE: return "TT_RBRACE";
    case TT_LSQUARE: return "TT_LSQUARE";
    case TT_RSQUARE: return "TT_RSQUARE";
    case TT_AMPER: return "TT_AMPER";
    case TT_PIPE: return "TT_PIPE";
    case TT_SEMICOLON: return "TT_SEMICOLON";
    case TT_COMMA: return "TT_COMMA";
    case TT_DOT: return "TT_DOT";
    case TT_ARROW: return "TT_ARROW";
    case TT_ELLIPSIS: return "TT_ELLIPSIS";
    case TT_PLUS_PLUS: return "TT_PLUS_PLUS";
    case TT_MINUS_MINUS: return "TT_MINUS_MINUS";
  }
}

const char *tokenLexeme(Token *t) {
  char *buf = malloc((t->lexemeLen + 1) * sizeof(char));
  strncpy(buf, t->lexeme, t->lexemeLen);
  buf[t->lexemeLen] = '\0';
  return buf;
}

int tokenPrefixBp(Token *t) {
  switch (t->type) {
    case TT_PLUS_PLUS:
    case TT_MINUS_MINUS:
    case TT_PLUS:
    case TT_MINUS:
    case TT_BANG:
    case TT_TILDA:
    case TT_STAR:
    case TT_AMPER:
      return 25;
    default: return 0;
  }
}

int *tokenInfixBp(Token *t) {
  int *ret = malloc(2 * sizeof(int));
  switch (t->type) {
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
    case TT_PIPE_EQ: {
      ret[0] = 2;
      ret[1] = 1;
      break;
    }
    case TT_PIPE_PIPE: {
      ret[0] = 3;
      ret[1] = 4;
      break;
    }
    case TT_AMPER_AMPER: {
      ret[0] = 5;
      ret[1] = 6;
      break;
    }
    case TT_PIPE: {
      ret[0] = 7;
      ret[1] = 8;
      break;
    }
    case TT_CARAT: {
      ret[0] = 9;
      ret[1] = 10;
      break;
    }
    case TT_AMPER: {
      ret[0] = 11;
      ret[1] = 12;
      break;
    }
    case TT_EQ_EQ:
    case TT_BANG_EQ: {
      ret[0] = 13;
      ret[1] = 14;
      break;
    }
    case TT_LESS:
    case TT_LESS_EQ:
    case TT_GREATER:
    case TT_GREATER_EQ: {
      ret[0] = 15;
      ret[1] = 16;
      break;
    }
    case TT_LESS_LESS:
    case TT_GREATER_GREATER: {
      ret[0] = 17;
      ret[1] = 18;
      break;
    }
    case TT_PLUS:
    case TT_MINUS: {
      ret[0] = 19;
      ret[1] = 20;
      break;
    }
    case TT_STAR:
    case TT_SLASH:
    case TT_PERCENT: {
      ret[0] = 21;
      ret[1] = 22;
      break;
    }
    default: return NULL;
  }
  return ret;
}

int tokenPostfixBp(Token *t) {
  switch (t->type) {
    case TT_PLUS_PLUS:
    case TT_MINUS_MINUS:
      return 26;
    default: return -1;
  }
}

void tokenPrint(Token *t) {
  const char *indent = getPrettyIndent();
  ++indentSize;
  const char *indentx = getPrettyIndent();
  --indentSize;

  printf("Token {\n%stype: '%s',\n%slexeme: '%.*s'\n%s}", indentx, tokenType(t), indentx, t->lexemeLen, t->lexeme, indent); 
}

Lexer lexerNew(char *src) {
  Lexer lexer = {.start = src, .curr = src};
  return lexer;
}

TokenType lexStr(Lexer *lexer) {
  do ++lexer->curr;
  while (lexer->curr[0] != '"' && lexer->curr[0] != '\0');
  if (lexer->curr[0] == '\0') return TT_UNCLOSED_STR;
  ++lexer->curr;

  return TT_STR;
}

TokenType lexInt(Lexer *lexer) {
  while (lexer->curr[0] >= '0' && lexer->curr[0] <= '9') ++lexer->curr;
  return TT_INT;
}

TokenType lexChar(Lexer *lexer) {
  if (lexer->curr[1] == '\0' || lexer->curr[2] == '\0') return TT_UNCLOSED_CHAR;

  lexer->curr += 2;
  if (lexer->curr[-1] != '\'') {
    while (lexer->curr[0] != '\'' && lexer->curr[0] != '\0') ++lexer->curr;
    return TT_UNCLOSED_CHAR;
  }

  return TT_CHAR;
}

TokenType lexIdent(Lexer *lexer) {
  while (
    (lexer->curr[0] >= 'a' && lexer->curr[0] <= 'z')
      || (lexer->curr[0] >= 'A' && lexer->curr[0] <= 'Z')
      || (lexer->curr[0] >= '0' && lexer->curr[0] <= '9')
      || lexer->curr[0] == '_'
  ) ++lexer->curr;
  return TT_IDENT;
}

int testKeyword(Lexer *lexer, const char *keyword, int len) {
  if (strncmp(lexer->start, keyword, len) != 0) return 0;
  lexer->curr += len - 1;
  return 1;
}

Token lexerNext(Lexer *lexer) {
  while (lexer->curr[0] == ' ' || lexer->curr[0] == '\n' || lexer->curr[0] == '\t') ++lexer->curr;
  lexer->start = lexer->curr;
  ++lexer->curr;

  size_t len = lexer->curr - lexer->start;

  Token t = {.type = TT_UNKNOWN, .lexeme = lexer->start};
  if (!lexer->start[0]) t.type = TT_EOF;
  else if (testKeyword(lexer, "int", 3)) t.type = TT_INT_TYPE;
  else if (testKeyword(lexer, "char", 4)) t.type = TT_CHAR_TYPE;
  else if (testKeyword(lexer, "float", 5)) t.type = TT_FLOAT_TYPE;
  else if (testKeyword(lexer, "double", 6)) t.type = TT_DOUBLE_TYPE;
  else if (testKeyword(lexer, "long", 4)) t.type = TT_LONG;
  else if (testKeyword(lexer, "unsigned", 8)) t.type = TT_UNSIGNED;
  else if (testKeyword(lexer, "auto", 4)) t.type = TT_AUTO;
  else if (testKeyword(lexer, "break", 5)) t.type = TT_BREAK;
  else if (testKeyword(lexer, "case", 4)) t.type = TT_CASE;
  else if (testKeyword(lexer, "const", 5)) t.type = TT_CONST;
  else if (testKeyword(lexer, "continue", 8)) t.type = TT_CONTINUE;
  else if (testKeyword(lexer, "default", 7)) t.type = TT_DEFAULT;
  else if (testKeyword(lexer, "do", 2)) t.type = TT_DO;
  else if (testKeyword(lexer, "else", 4)) t.type = TT_ELSE;
  else if (testKeyword(lexer, "enum", 4)) t.type = TT_ENUM;
  else if (testKeyword(lexer, "extern", 6)) t.type = TT_EXTERN;
  else if (testKeyword(lexer, "for", 3)) t.type = TT_FOR;
  else if (testKeyword(lexer, "goto", 4)) t.type = TT_GOTO;
  else if (testKeyword(lexer, "if", 2)) t.type = TT_IF;
  else if (testKeyword(lexer, "register", 8)) t.type = TT_REGISTER;
  else if (testKeyword(lexer, "return", 6)) t.type = TT_RETURN;
  else if (testKeyword(lexer, "short", 5)) t.type = TT_SHORT;
  else if (testKeyword(lexer, "signed", 6)) t.type = TT_SIGNED;
  else if (testKeyword(lexer, "sizeof", 6)) t.type = TT_SIZEOF;
  else if (testKeyword(lexer, "static", 6)) t.type = TT_STATIC;
  else if (testKeyword(lexer, "struct", 6)) t.type = TT_STRUCT;
  else if (testKeyword(lexer, "switch", 6)) t.type = TT_SWITCH;
  else if (testKeyword(lexer, "typedef", 7)) t.type = TT_TYPEDEF;
  else if (testKeyword(lexer, "union", 5)) t.type = TT_UNION;
  else if (testKeyword(lexer, "void", 4)) t.type = TT_VOID_TYPE;
  else if (testKeyword(lexer, "volatile", 8)) t.type = TT_VOLATILE;
  else if (testKeyword(lexer, "while", 5)) t.type = TT_WHILE;
  else if (lexer->start[0] == '"') t.type = lexStr(lexer);
  else if (lexer->start[0] >= '0' && lexer->start[0] <= '9') t.type = lexInt(lexer);
  else if (lexer->start[0] == '\'') t.type = lexChar(lexer);
  else if ((lexer->start[0] >= 'a' && lexer->start[0] <= 'z') || (lexer->start[0] >= 'A' && lexer->start[0] <= 'Z') || lexer->start[0] == '_') t.type = lexIdent(lexer);
  else if (!strncmp(lexer->start, "+=", len+1)) { ++lexer->curr; t.type = TT_PLUS_EQ; }
  else if (!strncmp(lexer->start, "++", len+1)) { ++lexer->curr; t.type = TT_PLUS_PLUS; }
  else if (!strncmp(lexer->start, "+", len)) t.type = TT_PLUS;
  else if (!strncmp(lexer->start, "-=", len+1)) { ++lexer->curr; t.type = TT_MINUS_EQ; }
  else if (!strncmp(lexer->start, "--", len+1)) { ++lexer->curr; t.type = TT_MINUS_MINUS; }
  else if (!strncmp(lexer->start, "->", len+1)) { ++lexer->curr; t.type = TT_ARROW; }
  else if (!strncmp(lexer->start, "-", len)) t.type = TT_MINUS;
  else if (!strncmp(lexer->start, "*=", len+1)) { ++lexer->curr; t.type = TT_STAR_EQ; }
  else if (!strncmp(lexer->start, "*", len)) t.type = TT_STAR;
  else if (!strncmp(lexer->start, "/=", len+1)) { ++lexer->curr; t.type = TT_SLASH_EQ; }
  else if (!strncmp(lexer->start, "/", len)) t.type = TT_SLASH;
  else if (!strncmp(lexer->start, "<<=", len+2)) { lexer->curr += 2; t.type = TT_LESS_LESS_EQ; }
  else if (!strncmp(lexer->start, "<<", len+1)) { ++lexer->curr; t.type = TT_LESS_LESS; }
  else if (!strncmp(lexer->start, "<=", len+1)) { ++lexer->curr; t.type = TT_LESS_EQ; }
  else if (!strncmp(lexer->start, "<", len)) t.type = TT_LESS;
  else if (!strncmp(lexer->start, ">>=", len+2)) { lexer->curr += 2; t.type = TT_GREATER_GREATER_EQ; }
  else if (!strncmp(lexer->start, ">>", len+1)) { ++lexer->curr; t.type = TT_GREATER_GREATER; }
  else if (!strncmp(lexer->start, ">=", len+1)) { ++lexer->curr; t.type = TT_GREATER_EQ; }
  else if (!strncmp(lexer->start, ">", len)) t.type = TT_GREATER;
  else if (!strncmp(lexer->start, "==", len+1)) { ++lexer->curr; t.type = TT_EQ_EQ; }
  else if (!strncmp(lexer->start, "=", len)) t.type = TT_EQ;
  else if (!strncmp(lexer->start, "!=", len+1)) { ++lexer->curr; t.type = TT_BANG_EQ; }
  else if (!strncmp(lexer->start, "!", len)) t.type = TT_BANG;
  else if (!strncmp(lexer->start, "&&", len+1)) { ++lexer->curr; t.type = TT_AMPER_AMPER; }
  else if (!strncmp(lexer->start, "||", len+1)) { ++lexer->curr; t.type = TT_PIPE_PIPE; }
  else if (!strncmp(lexer->start, "~", len)) t.type = TT_TILDA;
  else if (!strncmp(lexer->start, "^=", len+1)) { ++lexer->curr; t.type = TT_CARAT_EQ; }
  else if (!strncmp(lexer->start, "^", len)) t.type = TT_CARAT;
  else if (!strncmp(lexer->start, "%=", len+1)) { ++lexer->curr; t.type = TT_PERCENT_EQ; }
  else if (!strncmp(lexer->start, "%", len)) t.type = TT_PERCENT;
  else if (!strncmp(lexer->start, "(", len)) t.type = TT_LPAREN;
  else if (!strncmp(lexer->start, ")", len)) t.type = TT_RPAREN;
  else if (!strncmp(lexer->start, "{", len)) t.type = TT_LBRACE;
  else if (!strncmp(lexer->start, "}", len)) t.type = TT_RBRACE;
  else if (!strncmp(lexer->start, "[", len)) t.type = TT_LSQUARE;
  else if (!strncmp(lexer->start, "]", len)) t.type = TT_RSQUARE;
  else if (!strncmp(lexer->start, "&=", len+1)) { ++lexer->curr; t.type = TT_AMPER_EQ; }
  else if (!strncmp(lexer->start, "&", len)) t.type = TT_AMPER;
  else if (!strncmp(lexer->start, "|=", len+1)) { ++lexer->curr; t.type = TT_PIPE_EQ; }
  else if (!strncmp(lexer->start, "|", len)) t.type = TT_PIPE;
  else if (!strncmp(lexer->start, ";", len)) t.type = TT_SEMICOLON;
  else if (!strncmp(lexer->start, ",", len)) t.type = TT_COMMA;
  else if (!strncmp(lexer->start, "...", len+2)) { lexer->curr += 2; t.type = TT_ELLIPSIS; }
  else if (!strncmp(lexer->start, ".", len)) t.type = TT_DOT;
  t.lexemeLen = lexer->curr - lexer->start;

  return t;
}

Token lexerPeek(Lexer *lexer) {
  char *start = lexer->start;
  char *curr = lexer->curr;
  Token t = lexerNext(lexer);
  lexer->start = start;
  lexer->curr = curr;
  return t;
}
