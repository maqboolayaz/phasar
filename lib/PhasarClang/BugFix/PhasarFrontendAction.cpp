#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "PhasarASTConsumer.cpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include "BugFix.h"

// For each source file provided to the tool, a new FrontendAction is created.
class PhasarFrontendAction : public clang::ASTFrontendAction {
public:
  PhasarFrontendAction(BugFix bugFix)
      : _bugFix(bugFix) {}
  void EndSourceFileAction() override {
    clang::SourceManager &SM = TheRewriter.getSourceMgr();
    llvm::outs() << "** EndSourceFileAction for: "
                 << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";
    // Now emit the rewritten buffer.
    TheRewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
  }
  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &CI,
                    llvm::StringRef File) override {
    llvm::outs() << "** Creating AST consumer for: " << File << "\n";
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<PhasarASTConsumer>(TheRewriter, _bugFix);
  }
private:
  BugFix _bugFix;
  clang::Rewriter TheRewriter;
};

template <typename T, typename... Args>
std::unique_ptr<clang::tooling::FrontendActionFactory>
newFrontendActionWithParams(Args &&... args) {
  class SimpleFrontendActionFactory
      : public clang::tooling::FrontendActionFactory {
  public:
    SimpleFrontendActionFactory(Args &&... args)
        : args(std::forward<Args>(args)...) {}
    std::unique_ptr<clang::FrontendAction> create() override {
      auto FoldExpr = [](auto... Vs) {return (Vs, ...);};
      return std::make_unique<T>(std::apply(FoldExpr, args));
    }
  private:
    std::tuple<Args...> args;
  };
  return std::unique_ptr<clang::tooling::FrontendActionFactory>(
      new SimpleFrontendActionFactory(std::forward<Args>(args)...));
}