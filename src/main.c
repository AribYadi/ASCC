#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "lexer.h"

char *readFile(char *path);

int main(int argc, char **argv) {
  char *src = readFile(argv[1]);

  Lexer lexer = lexerNew(src);
  Token t = lexerNext(&lexer);
  do tokenPrint(t);
  while ((t = lexerNext(&lexer)).type != TT_EOF);

  free(src);
  return 0;
}

char *readFile(char *path) {
  char *buf = NULL;
  size_t len;
  FILE *f = fopen(path, "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc(len + 1);
    if (buf) {
      fread(buf, 1, len, f);
      buf[len] = '\0';
    }
    fclose(f);
  }

  return buf;
}
