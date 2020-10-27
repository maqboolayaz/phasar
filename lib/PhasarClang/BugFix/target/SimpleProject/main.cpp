/*
 * ============================================================================
 *
 * Philipp Schubert
 *
 *    Copyright (C) 2020
 *    Software Engineering Group
 *    Heinz Nixdorf Institut
 *    University of Paderborn
 *    philipp.schubert@upb.de
 *
 * ============================================================================
 */

#include <iostream>

#include "myfunctions.h"

using namespace std;

int main() {
  int x = 42;
  int y = 13;
  if (y) {
    int z = add(x, y);
  } else {
    cout << "x + y = " << x + y << '\n';
  }
  return 0;
}
