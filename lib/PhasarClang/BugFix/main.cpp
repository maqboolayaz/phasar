
#include <iostream>
#include <string>
#include "FixErrorsInClangAST.h"

// call with: './main -p=/home/philipp/ownCloud/Code/clang/clang-ast/target/SimpleProject /home/philipp/ownCloud/Code/clang/clang-ast/target/SimpleProject/myfunctions.cpp /home/philipp/ownCloud/Code/clang/clang-ast/target/SimpleProject/main.cpp'
int main(int argc, const char **argv) {

  const char* filePath ("/home/ubuntu/Desktop/phasar/lib/PhasarClang/BugFix/target/EvenSimplerProject/main.cpp");
  std::string error_statment ("int x = 42;");
  std::string fix_statement ("int x = 92;");

  psr::FixErrorsInClangAST fixingInterface;
  fixingInterface.modifyFixInClangAST(error_statment, fix_statement, filePath);

  //fixingInterface.deleteFixInClangAST(error_statment, filePath);

  //fixingInterface.addFixInClangAST(error_statment, fix_statement, filePath);
}