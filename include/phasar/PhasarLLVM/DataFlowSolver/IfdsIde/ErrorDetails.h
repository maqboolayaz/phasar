#pragma once
#include <string>

struct ErrorDetails{
  int lineNumber = 0;
  std::string sourceCode;
  std::string codeIR;
};