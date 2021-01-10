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
#include "phasar/Config/Configuration.h"

#include "gtest/gtest.h"

using namespace psr;


TEST(PhasarClangBugFixTest, HandleBugFix_01) {
// Testing the add AST functionality of PhasarClang Interface

  const std::string filePath =
    PhasarConfig::getPhasarConfig().PhasarDirectory() + 
    "lib/PhasarClang/BugFix/target/bugfix_01.cpp";
  std::string error_statment ("int x = 42;");
  std::string fix_statement ("int x = 92;");

  psr::FixErrorsInClangAST fixingInterface;

  fixingInterface.addFixInClangAST(error_statment, fix_statement, filePath.c_str());
}

TEST(PhasarClangBugFixTest, HandleBugFix_02) {
  // Testing the delete AST functionality of PhasarClang Interface

  const std::string filePath =
    PhasarConfig::getPhasarConfig().PhasarDirectory() + 
    "lib/PhasarClang/BugFix/target/bugfix_02.cpp";
  std::string error_statment ("fclose(f);");

  psr::FixErrorsInClangAST fixingInterface;

  fixingInterface.deleteFixInClangAST(error_statment, filePath.c_str());
}

TEST(PhasarClangBugFixTest, HandleBugFix_03) {
  // Testing the modify AST functionality of PhasarClang Interface

  const std::string filePath =
    PhasarConfig::getPhasarConfig().PhasarDirectory() + 
    "lib/PhasarClang/BugFix/target/bugfix_03.cpp";
  std::string error_statment ("fclose(f);");
  std::string fix_statement ("//fclose(f);");

  psr::FixErrorsInClangAST fixingInterface;
  //fixingInterface.modifyFixInClangAST(error_statment, fix_statement, filePath.c_str());
}

// main function for the test case
int main(int Argc, char **Argv) {
  ::testing::InitGoogleTest(&Argc, Argv);
  return RUN_ALL_TESTS();
}
