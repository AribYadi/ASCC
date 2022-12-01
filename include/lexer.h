#ifndef ASCC_LEXER_H
#define ASCC_LEXER_H

#include <stdint.h>

typedef enum {
  TT_EOF,
  TT_UNKNOWN,
  TT_UNCLOSED_STR,

  TT_INT,
  TT_STR,

  TT_PLUS,
  TT_MINUS,
  TT_STAR,
  TT_SLASH,
} TokenType;

typedef struct {
  TokenType type;
  char *lexeme;
  int lexemeLen;
} Token;

void tokenPrint(Token t);

typedef struct {
  char *start;
  char *curr;
} Lexer;

Lexer lexerNew(char *src);
Token lexerNext(Lexer *lexer);

#endif
