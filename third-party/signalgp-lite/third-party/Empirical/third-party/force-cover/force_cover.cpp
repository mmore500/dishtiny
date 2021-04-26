//------------------------------------------------------------------------------
// Force test coverage in C++
//
// This code modifies C++ source code to force test coverage
//
//
// Adapted from AST matching sample by
// Eli Bendersky (eliben@gmail.com)
//------------------------------------------------------------------------------
#include <string>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/AST/Decl.h"

#include <iostream>
#include <set>
#include <fstream>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory MatcherSampleCategory("Matcher Sample");

class TemplateClassHandler : public MatchFinder::MatchCallback {
public:
  TemplateClassHandler(Rewriter &Rewrite) : Rewrite(Rewrite) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    const CXXMethodDecl *ClassNode = Result.Nodes.getNodeAs<CXXMethodDecl>("class");
    
    if (ClassNode->isImplicit() || !ClassNode->getLocation().isValid() || ClassNode->isFromASTFile() || !ClassNode->isThisDeclarationADefinition()) {
      return;
    }

    std::string source_range = clang::Lexer::getSourceText(clang::CharSourceRange::getCharRange(ClassNode->getSourceRange()), Rewrite.getSourceMgr(), Rewrite.getLangOpts()).str();
    std::size_t loc = source_range.find_first_of("{");
    if (loc == std::string::npos) {
      loc = 0;
    } else {
      loc++;
    }

    Rewrite.InsertText(ClassNode->getLocStart().getLocWithOffset(loc), "/*_FORCE_COVER_START_*/", true, true);
    Rewrite.InsertText(ClassNode->getLocEnd().getLocWithOffset(1), "/*_FORCE_COVER_END_*/", true, true);
  }

private:
  Rewriter &Rewrite;
};



class TemplateFunctionHandler : public MatchFinder::MatchCallback {
public:
  TemplateFunctionHandler(Rewriter &Rewrite) : Rewrite(Rewrite) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    const FunctionTemplateDecl *FunctionNode = Result.Nodes.getNodeAs<FunctionTemplateDecl>("fun");
    
    if (FunctionNode->isImplicit() || !FunctionNode->getLocation().isValid() || !FunctionNode->isThisDeclarationADefinition()) {
      return;
    }

    std::string source_range = clang::Lexer::getSourceText(clang::CharSourceRange::getCharRange(FunctionNode->getSourceRange()), Rewrite.getSourceMgr(), Rewrite.getLangOpts()).str();
    std::size_t loc = source_range.find_first_of("{");
    if (loc == std::string::npos) {
      loc = 0;
    } else {
      loc++;
    }

    Rewrite.InsertText(FunctionNode->getLocStart().getLocWithOffset(loc), "/*_FORCE_COVER_START_*/", true, true);
    Rewrite.InsertText(FunctionNode->getLocEnd().getLocWithOffset(1), "/*_FORCE_COVER_END_*/", true, true);

  }

private:
  Rewriter &Rewrite;
};

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser. It registers a couple of matchers and runs them on
// the AST.
class MyASTConsumer : public ASTConsumer {
public:
  MyASTConsumer(Rewriter &R) : HandlerForTemplateClass(R),  HandlerForTemplateFunction(R) {

    // Match methods of templated classes
    Matcher.addMatcher(cxxMethodDecl(anyOf(hasAncestor(classTemplateDecl()), hasAncestor(classTemplateSpecializationDecl()))).bind("class"), &HandlerForTemplateClass);
    
    // Match templated functions
    Matcher.addMatcher(functionTemplateDecl().bind("fun"), &HandlerForTemplateFunction);

  }

  void HandleTranslationUnit(ASTContext &Context) override {
    // Run the matchers when we have the whole TU parsed.
    Matcher.matchAST(Context);
  }

private:
  TemplateClassHandler HandlerForTemplateClass;
  TemplateFunctionHandler HandlerForTemplateFunction;
  
  MatchFinder Matcher;
};

// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction : public ASTFrontendAction {
public:
  MyFrontendAction() {}
  void EndSourceFileAction() override {
    TheRewriter.getEditBuffer(TheRewriter.getSourceMgr().getMainFileID())
        .write(llvm::outs());
  }

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return llvm::make_unique<MyASTConsumer>(TheRewriter);
  }

private:
  Rewriter TheRewriter;
};

int main(int argc, const char **argv) {
  CommonOptionsParser op(argc, argv, MatcherSampleCategory);
  ClangTool Tool(op.getCompilations(), op.getSourcePathList());

  return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}
