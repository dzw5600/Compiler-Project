#include "semanticAnalyzer.h"
#include <iostream>
#include <typeinfo>

void SemanticAnalyzer::analyze(const std::vector<ParserNode*> &asts) {
    tables.enterScope(); // global
    for (auto node : asts) visit(node);
    tables.exitScope();
}

void SemanticAnalyzer::visit(ParserNode *node) {
    if (auto d = dynamic_cast<DeclarationNode*>(node))     visitDecl(d);
    else if (auto a = dynamic_cast<AssignmentNode*>(node)) visitAsgn(a);
    else if (auto v = dynamic_cast<VariableNode*>(node))   visitVar(v);
    else if (auto b = dynamic_cast<BinOpNode*>(node))      visitBinOp(b);
    else if (auto i = dynamic_cast<IfNode*>(node))         visitIf(i);
    else if (auto w = dynamic_cast<WhileLoopNode*>(node))  visitWhile(w);
    else if (auto f = dynamic_cast<FunctionDeclaration*>(node)) visitFuncDecl(f);
    else if (auto c = dynamic_cast<FunctionCall*>(node))   visitFuncCall(c);
    // literals / others: no action
}

void SemanticAnalyzer::visitDecl(DeclarationNode *d) {
    tables.declare(d->type, d->name);
}

void SemanticAnalyzer::visitAsgn(AssignmentNode *a) {
    auto lhsType = tables.lookup(a->var->name);
    if (lhsType.empty()) {
        std::cerr << "Semantic error: use of undeclared variable '"
                  << a->var->name << "'\n";
        return;
    }
    auto rhsType = exprType(a->value);
    if (rhsType.empty()) return;
    if (lhsType != rhsType) {
        std::cerr << "Type error: cannot assign '" << rhsType
                  << "' to '" << lhsType << "'\n";
    }
}

void SemanticAnalyzer::visitVar(VariableNode *v) {
    if (tables.lookup(v->name).empty()) {
        std::cerr << "Semantic error: undeclared variable '"
                  << v->name << "'\n";
    }
}

void SemanticAnalyzer::visitBinOp(BinOpNode *b) {
    auto lt = exprType(b->left), rt = exprType(b->right);
    auto op = b->op->getOperatorString();
    if (lt.empty() || rt.empty()) return;
    if ((op=="+"||op=="-"||op=="*"||op=="/"||op=="%")) {
        if ((lt!="int" && lt!="float") || rt!=lt) {
            std::cerr << "Type error: operator '"<<op
                      <<"' requires two numeric operands of same type\n";
        }
    } else if (op=="="||op=="!="||op=="<"||op=="<="||op==">"||op==">=") {
        if (lt!=rt) {
            std::cerr << "Type error: comparison '"<<op
                      <<"' between incompatible types\n";
        }
    } else if (op=="&&"||op=="||"||op=="!") {
        if (lt!="bool" || rt!="bool") {
            std::cerr << "Type error: logical '"<<op
                      <<"' requires boolean operands\n";
        }
    }
}

void SemanticAnalyzer::visitIf(IfNode *i) {
    if (exprType(i->condition) != "bool") {
        std::cerr << "Type error: if-condition not boolean\n";
    }
    tables.enterScope();
    for (auto stmt : i->thenBranch) visit(stmt);
    tables.exitScope();
    if (!i->elseBranch.empty()) {
        tables.enterScope();
        for (auto stmt : i->elseBranch) visit(stmt);
        tables.exitScope();
    }
}

void SemanticAnalyzer::visitWhile(WhileLoopNode *w) {
    if (exprType(w->condition) != "bool") {
        std::cerr << "Type error: while-condition not boolean\n";
    }
    tables.enterScope();
    for (auto stmt : w->statements) visit(stmt);
    tables.exitScope();
}

void SemanticAnalyzer::visitFuncDecl(FunctionDeclaration *f) {
    tables.declare("func", f->name);
    tables.enterScope();
    for (auto &p : f->parameters)
        tables.declare("param", p);
    for (auto stmt : f->body) visit(stmt);
    tables.exitScope();
}

void SemanticAnalyzer::visitFuncCall(FunctionCall *c) {
    if (tables.lookup(c->name).empty()) {
        std::cerr << "Semantic error: call to undefined function '"
                  << c->name << "'\n";
    }
    for (auto arg : c->arguments) exprType(arg);
}

std::string SemanticAnalyzer::exprType(ParserNode *n) {
    if (auto num = dynamic_cast<NumberNode*>(n)) {
        return num->type==TokenType::FLOAT_LITERAL ? "float" : "int";
    }
    if (auto str = dynamic_cast<StringNode*>(n)) { return "string"; }
    if (auto ch  = dynamic_cast<CharNode*>(n))   { return "char";   }
    if (auto b   = dynamic_cast<BooleanNode*>(n)){ return "bool";   }
    if (auto v   = dynamic_cast<VariableNode*>(n)) {
        return tables.lookup(v->name);
    }
    if (auto bin = dynamic_cast<BinOpNode*>(n)) {
        visitBinOp(bin);
        auto lt = exprType(bin->left), op = bin->op->getOperatorString();
        if (op=="+"||op=="-"||op=="*"||op=="/"||op=="%") return lt;
        return "bool";
    }
    if (auto call = dynamic_cast<FunctionCall*>(n)) {
        visitFuncCall(call);
        return "int"; // no return-type tracking yet
    }
    return "";
}
