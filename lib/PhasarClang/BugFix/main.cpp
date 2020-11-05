#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/StringRef.h"
#include "PhasarFrontendAction.cpp"
#include "clang/AST/ASTConsumer.h"
#include "BugFix.h"

static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");

// call with: './main -p=/home/philipp/ownCloud/Code/clang/clang-ast/target/SimpleProject /home/philipp/ownCloud/Code/clang/clang-ast/target/SimpleProject/myfunctions.cpp /home/philipp/ownCloud/Code/clang/clang-ast/target/SimpleProject/main.cpp'
int main(int argc, const char **argv) {
  clang::tooling::CommonOptionsParser Op(argc, argv, ToolingSampleCategory);
  clang::tooling::ClangTool Tool(Op.getCompilations(), Op.getSourcePathList());
  llvm::outs() << "FILES:\n";
  for (auto &Src : Op.getSourcePathList()) {
    llvm::outs() << Src << '\n';
  }
  llvm::outs() << "\n\n";

/*   BugFix fix{
    .error_statement = "int x = 42;",
    .fix_statement = "fix is here",
    .fix_operation = OPERATION::MODIFY}; */

  BugFix fix{
    .error_statement = "int x = 42;",
    .fix_statement = "int x = 92;",
    .fix_operation = OPERATION::ADD};

/*   BugFix fix{
    .error_statement = "int x = 42;",
    .fix_operation = OPERATION::DELETE}; */

  return Tool.run(
      newFrontendActionWithParams<PhasarFrontendAction>(fix).get());

}



