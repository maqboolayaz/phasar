/******************************************************************************
 * Copyright (c) 2020 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

#include <iostream>
#include <string>
#include "phasar/PhasarClang/BugFix/BugFix.h"
#include "phasar/PhasarClang/BugFix/FixErrorsInClangAST.h"
#include "clang/Tooling/Tooling.h"
#include "PhasarFrontendAction.cpp"
#include "clang/Tooling/CommonOptionsParser.h"


using namespace psr;

namespace psr {

static llvm::cl::OptionCategory ToolingCategory("PhASAR - Automatic Bug Repair");

FixErrorsInClangAST::FixErrorsInClangAST()
{
}

int FixErrorsInClangAST::modifyFixInClangAST(std::string error_statement, std::string fix_statement, const char* filePath) 
{
    BugFix fix{
        .error_statement = error_statement,
        .fix_statement = fix_statement,
        .fix_operation = OPERATION::MODIFY};

    const char* argsOP[]={
        ".", filePath,
    };
    const char **argvOP = argsOP;
    int argcOP = sizeof(argsOP)/sizeof(char*);
    clang::tooling::CommonOptionsParser Op(argcOP, argvOP, ToolingCategory);
    clang::tooling::ClangTool Tool(Op.getCompilations(), Op.getSourcePathList());

    return Tool.run(
        newFrontendActionWithParams<PhasarFrontendAction>(fix).get());
}

int FixErrorsInClangAST::deleteFixInClangAST(std::string error_statement, const char* filePath) 
{
    BugFix fix{
        .error_statement = error_statement,
        .fix_operation = OPERATION::DELETE};

    const char* argsOP[]={
        ".", filePath,
    };
    const char **argvOP = argsOP;
    int argcOP = sizeof(argsOP)/sizeof(char*);
    clang::tooling::CommonOptionsParser Op(argcOP, argvOP, ToolingCategory);
    clang::tooling::ClangTool Tool(Op.getCompilations(), Op.getSourcePathList());

    return Tool.run(
        newFrontendActionWithParams<PhasarFrontendAction>(fix).get());
}

int FixErrorsInClangAST::addFixInClangAST(std::string error_statement, std::string fix_statement, const char* filePath) 
{
    BugFix fix{
        .error_statement = error_statement,
        .fix_statement = fix_statement,
        .fix_operation = OPERATION::ADD};

    const char* argsOP[]={
        ".", filePath,
    };
    const char **argvOP = argsOP;
    int argcOP = sizeof(argsOP)/sizeof(char*);
    clang::tooling::CommonOptionsParser Op(argcOP, argvOP, ToolingCategory);
    clang::tooling::ClangTool Tool(Op.getCompilations(), Op.getSourcePathList());

    return Tool.run(
        newFrontendActionWithParams<PhasarFrontendAction>(fix).get());
}

} // namespace psr
