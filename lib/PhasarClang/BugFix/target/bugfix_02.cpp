#include <stdio.h>

int func2() {
  FILE *f;
  f = fopen("file.txt", "r");
  fclose(f);
  fclose(f);
  return 0;
}