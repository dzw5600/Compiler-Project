#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

struct Symbol {
    std::string type;
    std::string name;
    Symbol(std::string t, std::string n) : type(t), name(n) {}
};

class SymbolTable {
public:
    std::unordered_map<std::string,Symbol> table;

    // declare a new symbol
    bool addSymbol(const std::string &type, const std::string &name) {
        if (table.count(name)) {
            std::cerr << "Semantic error: redeclaration of '" << name << "'\n";
            return false;
        }
        table[name] = Symbol(type,name);
        return true;
    }
    // lookup a symbol’s type, or empty if not found
    std::string lookup(const std::string &name) const {
        auto it = table.find(name);
        if (it != table.end()) return it->second.type;
        return "";
    }
};

class SymbolTableStack {
public:
    std::vector<SymbolTable> stack;

    void enterScope() {
        stack.emplace_back();
    }
    void exitScope() {
        stack.pop_back();
    }
    bool declare(const std::string &type, const std::string &name) {
        return stack.back().addSymbol(type,name);
    }
    // search from innermost outward
    std::string lookup(const std::string &name) const {
        for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
            auto t = it->lookup(name);
            if (!t.empty()) return t;
        }
        return "";
    }
};

#endif // SYMBOLTABLE_H
