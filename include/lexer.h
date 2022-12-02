#ifndef ASCC_LEXER_H
#define ASCC_LEXER_H

#include <stdint.h>

typedef enum {
  TT_EOF,
  TT_UNKNOWN,
  TT_UNCLOSED_STR,
  TT_UNCLOSED_CHAR,

  TT_INT_TYPE,
  TT_CHAR_TYPE,
  TT_FLOAT_TYPE,
  TT_DOUBLE_TYPE,
  TT_LONG,
  TT_UNSIGNED,

  TT_INT,
  TT_STR,
  TT_CHAR,
  TT_TRUE,
  TT_FALSE,

  TT_PLUS,
  TT_MINUS,
  TT_STAR,
  TT_SLASH,
  TT_LESS,
  TT_LESS_EQ,
  TT_GREATER,
  TT_GREATER_EQ,
  TT_EQ,
  TT_EQ_EQ,
  TT_BANG,
  TT_BANG_EQ,
  TT_AMPER_AMPER,
  TT_PIPE_PIPE,

  TT_LPAREN,
  TT_RPAREN,
  TT_LBRACE,
  TT_RBRACE,
  TT_LSQUARE,
  TT_RSQUARE,
  TT_AMPER,
  TT_PIPE,
  TT_SEMICOLON,
  TT_COMMA,
  TT_DOT,
  TT_ARROW,
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
