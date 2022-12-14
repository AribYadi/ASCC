#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "parser.h"
#include "globals.h"

int indentSize = 0;
const char *getPrettyIndent() {
  char *indent = malloc((indentSize * PRETTY_INDENT_SIZE + 1) * sizeof(char));
  for (int i = 0; i < indentSize * PRETTY_INDENT_SIZE; ++i) {
    indent[i] = ' ';
  }
  indent[indentSize * PRETTY_INDENT_SIZE] = '\0';
  return indent;
}

char *readFile(char *path);

int main(int argc, char **argv) {
  char *src = readFile(argv[1]);

  if (src) {
    StmtVec stmts = parse(src);
    stmtVecPrint(&stmts);
    printf("\n");
    stmtVecFree(stmts);

    free(src);
  }

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
