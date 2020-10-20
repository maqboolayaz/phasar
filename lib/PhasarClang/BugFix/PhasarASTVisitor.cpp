#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "clang/AST/AST.h"
#include "clang/AST/PrettyPrinter.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"


// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class PhasarASTVisitor : public clang::RecursiveASTVisitor<PhasarASTVisitor> {
public:
  PhasarASTVisitor(clang::Rewriter &R) : TheRewriter(R) {}

  bool VisitStmt(clang::Stmt *S) {
    // Only care about If statements.
    // std::cout << "Hello"<<std::endl;
    // clang::Stmt::StmtClass test = S->getStmtClass();
    // std::cout << test <<std::endl;

    std::string statement;
    llvm::raw_string_ostream stream(statement);
    S->printPretty(stream, NULL, clang::PrintingPolicy(clang::LangOptions()));
    stream.flush();
    std::cout << "Statement :" << statement <<std::endl;

    if(statement.find("int x = 42;") == 0){
      std::cout << "Hello"<<std::endl;
      clang::CharSourceRange range;
      range.setBegin(S->getBeginLoc());
      range.setEnd(S->getEndLoc());

      TheRewriter.ReplaceText(S->getSourceRange() , "int x = 1;");
    }


    if (clang::isa<clang::IfStmt>(S)) {
      clang::IfStmt *IfStatement = clang::cast<clang::IfStmt>(S);
      clang::Stmt *Then = IfStatement->getThen();

      TheRewriter.InsertText(Then->getBeginLoc(), "// the 'if' part\n", true,
                             true);

      clang::Stmt *Else = IfStatement->getElse();
      if (Else)
        TheRewriter.InsertText(Else->getBeginLoc(), "// the 'else' part\n",
                               true, true);
    }

    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *F) {
    // Only function definitions (with bodies), not declarations.
    if (F->hasBody()) {
      clang::Stmt *FunBody = F->getBody();

      // Type name as string
      clang::QualType QT = F->getReturnType();
      std::string TypeStr = QT.getAsString();

      // Function name
      clang::DeclarationName DeclName = F->getNameInfo().getName();
      std::string FuncName = DeclName.getAsString();

      // Add comment before
      std::stringstream SSBefore;
      SSBefore << "// Begin function " << FuncName << " returning " << TypeStr
               << "\n";
      clang::SourceLocation ST = F->getSourceRange().getBegin();
      TheRewriter.InsertText(ST, SSBefore.str(), true, true);

      // And after
      std::stringstream SSAfter;
      SSAfter << "\n// End function " << FuncName;
      ST = FunBody->getEndLoc().getLocWithOffset(1);
      TheRewriter.InsertText(ST, SSAfter.str(), true, true);
    }

    return true;
  }

private:
  clang::Rewriter &TheRewriter;
};
