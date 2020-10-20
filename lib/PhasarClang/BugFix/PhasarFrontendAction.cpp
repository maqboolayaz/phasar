#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "PhasarASTConsumer.cpp"

// For each source file provided to the tool, a new FrontendAction is created.
class PhasarFrontendAction : public clang::ASTFrontendAction {
public:
  PhasarFrontendAction() {}
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
    return std::make_unique<PhasarASTConsumer>(TheRewriter);
  }

private:
  clang::Rewriter TheRewriter;
};