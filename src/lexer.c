#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"

void tokenPrint(Token t) {
  printf("Token { type: '");
  switch (t.type) {
    case TT_UNKNOWN: printf("TT_UNKNOWN"); break;
    case TT_INT: printf("TT_INT"); break;
    case TT_PLUS: printf("TT_PLUS"); break;
    case TT_MINUS: printf("TT_MINUS"); break;
    case TT_STAR: printf("TT_STAR"); break;
    case TT_SLASH: printf("TT_SLASH"); break;
    case TT_EOF: printf("TT_EOF"); break;
  }
  printf("', lexeme: '%.*s' }\n", t.lexemeLen, t.lexeme); 
}

Lexer lexerNew(char *src) {
  Lexer lexer = {.start = src, .curr = src};
  return lexer;
}

TokenType lexInt(Lexer *lexer) {
  while (lexer->curr[0] >= '0' && lexer->curr[0] <= '9') ++lexer->curr;
  return TT_INT;
}

Token lexerNext(Lexer *lexer) {

  ++lexer->curr;
  size_t len = lexer->curr - lexer->start;

  Token t = {.type = TT_UNKNOWN, .lexeme = lexer->start};
  if (!lexer->start[0]) t.type = TT_EOF;
  else if (lexer->start[0] >= '0' && lexer->start[0] <= '9') t.type = lexInt(lexer);
  else if (!strncmp(lexer->start, "+", len)) t.type = TT_PLUS;
  else if (!strncmp(lexer->start, "-", len)) t.type = TT_MINUS;
  else if (!strncmp(lexer->start, "*", len)) t.type = TT_STAR;
  else if (!strncmp(lexer->start, "/", len)) t.type = TT_SLASH;
  t.lexemeLen = lexer->curr - lexer->start;

  lexer->start = lexer->curr;

  return t;
}
