#include <iostream>
#include <stddef.h>
#include <string>
#include <stdio.h>

int func3() {
  FILE *f;
  f = fopen("file.txt", "r");
  fclose(f);
  fclose(f);
  return 0;
}