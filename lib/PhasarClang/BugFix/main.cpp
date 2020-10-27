#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/StringRef.h"
#include "PhasarFrontendAction.cpp"
#include "clang/AST/ASTConsumer.h"
#include "BugFix.h"

static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");

class MyCallbacks : public clang::tooling::SourceFileCallbacks
{
  //virtual bool handleBeginSource(CompilerInstance &) override{ return true;}
  //virtual void handleEndSource() override{ 
  //  clang::SourceManager &SM = TheRewriter.getSourceMgr();
  //  llvm::outs() << "** EndSourceFileAction for: "
  //               << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";

    // Now emit the rewritten buffer.
  //  TheRewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
  //}
};

struct MyFrontEndActionFactory
{ 
  BugFix bugfix;
  clang::Rewriter TheRewriter;

  std::unique_ptr<clang::ASTConsumer> newASTConsumer()
  {
    return std::make_unique<PhasarASTConsumer>(TheRewriter, bugfix.fix_statement);
  }

};

// call with: './main -p=/home/philipp/ownCloud/Code/clang/clang-ast/target/SimpleProject /home/philipp/ownCloud/Code/clang/clang-ast/target/SimpleProject/myfunctions.cpp /home/philipp/ownCloud/Code/clang/clang-ast/target/SimpleProject/main.cpp'
int main(int argc, const char **argv) {
  clang::tooling::CommonOptionsParser Op(argc, argv, ToolingSampleCategory);
  clang::tooling::ClangTool Tool(Op.getCompilations(), Op.getSourcePathList());
  llvm::outs() << "FILES:\n";
  for (auto &Src : Op.getSourcePathList()) {
    llvm::outs() << Src << '\n';
  }
  llvm::outs() << "\n\n";

  BugFix fix{
    .error_statement = "erro_tesr",
    .fix_statement = "testing",
    .fix_operation = OPERATION::ADD};
  //MyCallbacks callbacks;
  //MyFrontEndActionFactory Factory;
  //Factory.bugfix = fix;
  //auto FactoryAdapter = clang::tooling::newFrontendActionFactory(&Factory, &callbacks);

  //return Tool.run(FactoryAdapter.get());
  return Tool.run(
    clang::tooling::newFrontendActionFactory<PhasarFrontendAction>().get());
}


