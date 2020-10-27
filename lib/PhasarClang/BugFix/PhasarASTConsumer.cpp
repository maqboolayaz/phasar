#include "clang/AST/ASTConsumer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/AST.h"

#include "PhasarASTVisitor.cpp"

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class PhasarASTConsumer : public clang::ASTConsumer {
public:
  PhasarASTConsumer(clang::Rewriter &R, std::string fix_statement) : Visitor(R, fix_statement) {}

  void HandleTranslationUnit(clang::ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  PhasarASTVisitor Visitor;
};