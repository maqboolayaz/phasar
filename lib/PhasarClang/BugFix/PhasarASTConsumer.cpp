#include "clang/AST/ASTConsumer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/AST.h"
#include "PhasarASTVisitor.cpp"
#include "BugFix.h"

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class PhasarASTConsumer : public clang::ASTConsumer {
public:
  PhasarASTConsumer(clang::Rewriter &R, BugFix bugFix) : Visitor(R, bugFix) {}

  void HandleTranslationUnit(clang::ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  PhasarASTVisitor Visitor;
};