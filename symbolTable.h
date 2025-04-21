#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

// Single symbol entry
struct Symbol {
    std::string type;
    std::string name;
    Symbol() : type(""), name("") {}
    Symbol(const std::string &type, const std::string &name)
      : type(type), name(name) {}
};

class SymbolTable {
public:
    // add a new symbol; reports error and returns false on redeclaration
    bool addSymbol(const std::string &type, const std::string &name) {
        if (table.find(name) != table.end()) {
            std::cerr << "Semantic error: redeclaration of '" << name << "'\n";
            return false;
        }
        table[name] = Symbol(type, name);
        return true;
    }

    // remove a symbol; reports error if not found
    void deleteSymbol(const std::string &name) {
        if (table.find(name) == table.end()) {
            std::cerr << "Semantic error: delete of unknown variable '" << name << "'\n";
            return;
        }
        table.erase(name);
    }

    // lookup a symbol’s type; empty string if not found
    std::string lookup(const std::string &name) const {
        auto it = table.find(name);
        if (it != table.end()) return it->second.type;
        return "";
    }

    // check existence without retrieving type
    bool contains(const std::string &name) const {
        return table.find(name) != table.end();
    }

private:
    std::unordered_map<std::string, Symbol> table;
};

class SymbolTableStack {
public:
    // enter a new (inner) scope
    void enterScope() {
        stack.emplace_back();
    }

    // exit current scope
    void exitScope() {
        if (!stack.empty()) stack.pop_back();
    }

    // declare in current scope
    bool declare(const std::string &type, const std::string &name) {
        if (stack.empty()) enterScope();
        return stack.back().addSymbol(type, name);
    }

    // delete from current scope
    void deleteCurrent(const std::string &name) {
        if (stack.empty()) {
            std::cerr << "Semantic error: no scope to delete from\n";
        } else {
            stack.back().deleteSymbol(name);
        }
    }

    // lookup a name’s type from innermost to outermost; empty if not found
    std::string lookup(const std::string &name) const {
        for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
            auto t = it->lookup(name);
            if (!t.empty()) return t;
        }
        return "";
    }

    // check existence in any scope
    bool contains(const std::string &name) const {
        for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
            if (it->contains(name)) return true;
        }
        return false;
    }

private:
    std::vector<SymbolTable> stack;
};

#endif // SYMBOLTABLE_H
