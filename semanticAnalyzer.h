#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include <vector>
#include <string>
#include "parser.h"
#include "symbolTable.h"

class SemanticAnalyzer {
public:
    // Analyze the AST, reporting any semantic errors
    void analyze(const std::vector<ParserNode*> &asts);

private:
    SymbolTableStack tables;

    // Dispatch to the right visitor
    void visit(ParserNode *node);

    // Specific node visitors
    void visitDecl(DeclarationNode *d);
    void visitAsgn(AssignmentNode *a);
    void visitVar(VariableNode *v);
    void visitBinOp(BinOpNode *b);
    void visitIf(IfNode *i);
    void visitWhile(WhileLoopNode *w);
    void visitFuncDecl(FunctionDeclaration *f);
    void visitFuncCall(FunctionCall *c);

    // Helpers
    std::string exprType(ParserNode *n);
};

#endif // SEMANTICANALYZER_H

