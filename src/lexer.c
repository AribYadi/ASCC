#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"

void tokenPrint(Token t) {
  switch (t) {
    case TT_UNKNOWN: printf("TT_UNKNOWN\n"); break;
    case TT_INT: printf("TT_INT\n"); break;
    case TT_PLUS: printf("TT_PLUS\n"); break;
    case TT_MINUS: printf("TT_MINUS\n"); break;
    case TT_STAR: printf("TT_STAR\n"); break;
    case TT_SLASH: printf("TT_SLASH\n"); break;
    case TT_EOF: printf("TT_EOF\n"); break;
  }
}

Lexer lexerNew(char *src) {
  Lexer lexer = {.start = src, .curr = src};
  return lexer;
}

Token lexerNext(Lexer *lexer) {
  if (!*lexer->start) return TT_EOF;

  ++lexer->curr;
  size_t len = lexer->curr - lexer->start;
  char *subs = malloc(len + 1);
  strncpy(subs, lexer->start, len);
  subs[len] = '\0';

  Token t = TT_UNKNOWN;
  if (!strcmp(subs, "+")) t = TT_PLUS;
  else if (!strcmp(subs, "-")) t = TT_MINUS;
  else if (!strcmp(subs, "*")) t = TT_STAR;
  else if (!strcmp(subs, "/")) t = TT_SLASH;

  lexer->start = lexer->curr;

  free(subs);
  return t;
}
