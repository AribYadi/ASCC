#include <stdio.h>

#include "lexer.h"

Lexer lexerNew(char *src) {
  Lexer lexer = {.src = src, .start = 0, .curr = 0};
  return lexer;
}

void tokenPrint(Token t) {
  switch (t) {
    case TT_UNKNOWN: printf("TT_UNKNOWN\n"); break;
    case TT_INT: printf("TT_INT\n"); break;
    case TT_PLUS: printf("TT_PLUS\n"); break;
    case TT_MINUS: printf("TT_MINUS\n"); break;
    case TT_STAR: printf("TT_STAR\n"); break;
    case TT_SLASH: printf("TT_SLASH\n"); break;
  }
}

Token lexerNext(Lexer *lexer) {
  return TT_UNKNOWN;
}
