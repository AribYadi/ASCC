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
    case TT_STR: printf("TT_STR"); break;
    case TT_INT: printf("TT_INT"); break;
    case TT_CHAR: printf("TT_CHAR"); break;
    case TT_PLUS: printf("TT_PLUS"); break;
    case TT_MINUS: printf("TT_MINUS"); break;
    case TT_STAR: printf("TT_STAR"); break;
    case TT_SLASH: printf("TT_SLASH"); break;
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

Token lexerNext(Lexer *lexer) {
  ++lexer->curr;
  size_t len = lexer->curr - lexer->start;

  Token t = {.type = TT_UNKNOWN, .lexeme = lexer->start};
  if (!lexer->start[0]) t.type = TT_EOF;
  else if (lexer->start[0] == '"') t.type = lexStr(lexer);
  else if (lexer->start[0] >= '0' && lexer->start[0] <= '9') t.type = lexInt(lexer);
  else if (lexer->start[0] == '\'') t.type = lexChar(lexer);
  else if (!strncmp(lexer->start, "+", len)) t.type = TT_PLUS;
  else if (!strncmp(lexer->start, "-", len)) t.type = TT_MINUS;
  else if (!strncmp(lexer->start, "*", len)) t.type = TT_STAR;
  else if (!strncmp(lexer->start, "/", len)) t.type = TT_SLASH;
  t.lexemeLen = lexer->curr - lexer->start;

  lexer->start = lexer->curr;

  return t;
}
