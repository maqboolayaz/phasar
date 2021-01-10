/******************************************************************************
 * Copyright (c) 2021 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

#include <iostream>
#include <string>
#include "phasar/PhasarClang/BugFix/FixErrorsInClangAST.h"

#include "gtest/gtest.h"

using namespace psr;


TEST(PhasarClangBugFixTest, HandleBugFix_01) {

  const char* filePath ("/home/ayaz/Desktop/phasar/lib/PhasarClang/BugFix/target/EvenSimplerProject/test.cpp");
  std::string error_statment ("int x = 42;");
  std::string fix_statement ("int x = 92;");

  psr::FixErrorsInClangAST fixingInterface;
  fixingInterface.modifyFixInClangAST(error_statment, fix_statement, filePath);

  //fixingInterface.deleteFixInClangAST(error_statment, filePath);

  //fixingInterface.addFixInClangAST(error_statment, fix_statement, filePath);
}

// main function for the test case
int main(int Argc, char **Argv) {
  ::testing::InitGoogleTest(&Argc, Argv);
  return RUN_ALL_TESTS();
}
