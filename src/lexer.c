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
    case TT_AUTO: printf("TT_AUTO"); break;
    case TT_BREAK: printf("TT_BREAK"); break;
    case TT_CASE: printf("TT_CASE"); break;
    case TT_CONST: printf("TT_CONST"); break;
    case TT_CONTINUE: printf("TT_CONTINUE"); break;
    case TT_DEFAULT: printf("TT_DEFAULT"); break;
    case TT_DO: printf("TT_DO"); break;
    case TT_ELSE: printf("TT_ELSE"); break;
    case TT_ENUM: printf("TT_ENUM"); break;
    case TT_EXTERN: printf("TT_EXTERN"); break;
    case TT_FOR: printf("TT_FOR"); break;
    case TT_GOTO: printf("TT_GOTO"); break;
    case TT_IF: printf("TT_IF"); break;
    case TT_REGISTER: printf("TT_REGISTER"); break;
    case TT_RETURN: printf("TT_RETURN"); break;
    case TT_SHORT: printf("TT_SHORT"); break;
    case TT_SIGNED: printf("TT_SIGNED"); break;
    case TT_SIZEOF: printf("TT_SIZEOF"); break;
    case TT_STATIC: printf("TT_STATIC"); break;
    case TT_STRUCT: printf("TT_STRUCT"); break;
    case TT_SWITCH: printf("TT_SWITCH"); break;
    case TT_TYPEDEF: printf("TT_TYPEDEF"); break;
    case TT_UNION: printf("TT_UNION"); break;
    case TT_VOID_TYPE: printf("TT_VOID_TYPE"); break;
    case TT_VOLATILE: printf("TT_VOLATILE"); break;
    case TT_WHILE: printf("TT_WHILE"); break;
    case TT_STR: printf("TT_STR"); break;
    case TT_INT: printf("TT_INT"); break;
    case TT_CHAR: printf("TT_CHAR"); break;
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
