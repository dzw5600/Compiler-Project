#include "symbolTable.h"

// Single-scope additions and removals

bool SymbolTable::addSymbol(const std::string &type, const std::string &name) {
    if (table.find(name) != table.end()) {
        std::cerr << "Semantic error: redeclaration of '" << name << "'\n";
        return false;
    }
    table[name] = Symbol(type, name);
    return true;
}

void SymbolTable::deleteSymbol(const std::string &name) {
    if (table.find(name) == table.end()) {
        std::cerr << "Semantic error: delete of unknown variable '" << name << "'\n";
        return;
    }
    table.erase(name);
}

std::string SymbolTable::lookup(const std::string &name) const {
    auto it = table.find(name);
    if (it != table.end()) return it->second.type;
    return "";
}

bool SymbolTable::contains(const std::string &name) const {
    return table.find(name) != table.end();
}

// Scope-stack operations

void SymbolTableStack::enterScope() {
    stack.emplace_back();
}

void SymbolTableStack::exitScope() {
    if (!stack.empty()) stack.pop_back();
}

bool SymbolTableStack::declare(const std::string &type, const std::string &name) {
    if (stack.empty()) enterScope();
    return stack.back().addSymbol(type, name);
}

void SymbolTableStack::deleteCurrent(const std::string &name) {
    if (stack.empty()) {
        std::cerr << "Semantic error: no scope to delete from\n";
    } else {
        stack.back().deleteSymbol(name);
    }
}

std::string SymbolTableStack::lookup(const std::string &name) const {
    for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
        auto t = it->lookup(name);
        if (!t.empty()) return t;
    }
    return "";
}

bool SymbolTableStack::contains(const std::string &name) const {
    for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
        if (it->contains(name)) return true;
    }
    return false;
}
