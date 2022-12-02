#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"

void tokenPrint(Token t) {
  printf("Token { type: '");
  switch (t.type) {
    case TT_EOF: printf("TT_EOF"); break;
    case TT_UNKNOWN: printf("TT_UNKNOWN"); break;
    case TT_UNCLOSED_STR: printf("TT_UNCLOSED_STR"); break;
    case TT_UNCLOSED_CHAR: printf("TT_UNCLOSED_CHAR"); break;
    case TT_INT_TYPE: printf("TT_INT_TYPE"); break;
    case TT_CHAR_TYPE: printf("TT_CHAR_TYPE"); break;
    case TT_FLOAT_TYPE: printf("TT_FLOAT_TYPE"); break;
    case TT_DOUBLE_TYPE: printf("TT_DOUBLE_TYPE"); break;
    case TT_LONG: printf("TT_LONG"); break;
    case TT_UNSIGNED: printf("TT_UNSIGNED"); break;
    case TT_STR: printf("TT_STR"); break;
    case TT_INT: printf("TT_INT"); break;
    case TT_CHAR: printf("TT_CHAR"); break;
    case TT_TRUE: printf("TT_TRUE"); break;
    case TT_FALSE: printf("TT_FALSE"); break;
    case TT_PLUS: printf("TT_PLUS"); break;
    case TT_MINUS: printf("TT_MINUS"); break;
    case TT_STAR: printf("TT_STAR"); break;
    case TT_SLASH: printf("TT_SLASH"); break;
    case TT_LESS: printf("TT_LESS"); break;
    case TT_LESS_EQ: printf("TT_LESS_EQ"); break;
    case TT_GREATER: printf("TT_GREATER"); break;
    case TT_GREATER_EQ: printf("TT_GREATER_EQ"); break;
    case TT_EQ: printf("TT_EQ"); break;
    case TT_EQ_EQ: printf("TT_EQ_EQ"); break;
    case TT_BANG: printf("TT_BANG"); break;
    case TT_BANG_EQ: printf("TT_BANG_EQ"); break;
    case TT_AMPER_AMPER: printf("TT_AMPER_AMPER"); break;
    case TT_PIPE_PIPE: printf("TT_PIPE_PIPE"); break;
    case TT_LPAREN: printf("TT_LPAREN"); break;
    case TT_RPAREN: printf("TT_RPAREN"); break;
    case TT_LBRACE: printf("TT_LBRACE"); break;
    case TT_RBRACE: printf("TT_RBRACE"); break;
    case TT_LSQUARE: printf("TT_LSQUARE"); break;
    case TT_RSQUARE: printf("TT_RSQUARE"); break;
    case TT_AMPER: printf("TT_AMPER"); break;
    case TT_PIPE: printf("TT_PIPE"); break;
    case TT_SEMICOLON: printf("TT_SEMICOLON"); break;
    case TT_COMMA: printf("TT_COMMA"); break;
    case TT_DOT: printf("TT_DOT"); break;
    case TT_ARROW: printf("TT_ARROW"); break;
  }
  printf("', lexeme: '%.*s' }\n", t.lexemeLen, t.lexeme); 
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
  else if (lexer->start[0] == '"') t.type = lexStr(lexer);
  else if (lexer->start[0] >= '0' && lexer->start[0] <= '9') t.type = lexInt(lexer);
  else if (lexer->start[0] == '\'') t.type = lexChar(lexer);
  else if (testKeyword(lexer, "true", 4)) t.type = TT_TRUE;
  else if (testKeyword(lexer, "false", 5)) t.type = TT_FALSE;
  else if (!strncmp(lexer->start, "+", len)) t.type = TT_PLUS;
  else if (!strncmp(lexer->start, "->", len+1)) { ++lexer->curr; t.type = TT_ARROW; }
  else if (!strncmp(lexer->start, "-", len)) t.type = TT_MINUS;
  else if (!strncmp(lexer->start, "*", len)) t.type = TT_STAR;
  else if (!strncmp(lexer->start, "/", len)) t.type = TT_SLASH;
  else if (!strncmp(lexer->start, "<=", len+1)) { ++lexer->curr; t.type = TT_LESS_EQ; }
  else if (!strncmp(lexer->start, "<", len)) t.type = TT_LESS;
  else if (!strncmp(lexer->start, ">=", len+1)) { ++lexer->curr; t.type = TT_GREATER_EQ; }
  else if (!strncmp(lexer->start, ">", len)) t.type = TT_GREATER;
  else if (!strncmp(lexer->start, "==", len+1)) { ++lexer->curr; t.type = TT_EQ_EQ; }
  else if (!strncmp(lexer->start, "=", len)) t.type = TT_EQ;
  else if (!strncmp(lexer->start, "!=", len+1)) { ++lexer->curr; t.type = TT_BANG_EQ; }
  else if (!strncmp(lexer->start, "!", len)) t.type = TT_BANG;
  else if (!strncmp(lexer->start, "&&", len+1)) { ++lexer->curr; t.type = TT_AMPER_AMPER; }
  else if (!strncmp(lexer->start, "||", len+1)) { ++lexer->curr; t.type = TT_PIPE_PIPE; }
  else if (!strncmp(lexer->start, "(", len)) t.type = TT_LPAREN;
  else if (!strncmp(lexer->start, ")", len)) t.type = TT_RPAREN;
  else if (!strncmp(lexer->start, "{", len)) t.type = TT_LBRACE;
  else if (!strncmp(lexer->start, "}", len)) t.type = TT_RBRACE;
  else if (!strncmp(lexer->start, "[", len)) t.type = TT_LSQUARE;
  else if (!strncmp(lexer->start, "]", len)) t.type = TT_RSQUARE;
  else if (!strncmp(lexer->start, "&", len)) t.type = TT_AMPER;
  else if (!strncmp(lexer->start, "|", len)) t.type = TT_PIPE;
  else if (!strncmp(lexer->start, ";", len)) t.type = TT_SEMICOLON;
  else if (!strncmp(lexer->start, ",", len)) t.type = TT_COMMA;
  else if (!strncmp(lexer->start, ".", len)) t.type = TT_DOT;
  t.lexemeLen = lexer->curr - lexer->start;

  return t;
}
