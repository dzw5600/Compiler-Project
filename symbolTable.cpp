#include "symbolTable.h"

class Symbol
{
public:
    std::string type;
    std::string name;
    Symbol(std::string type, std::string name)
    {
        this->type = type;
        this->name = name;
    }
};

class SymbolTable
{
public:
    std::unordered_map<std::string, Symbol> table;

    void addSymbol(std::string type, std::string name)
    {
        if (table.find(name) != table.end())
        {
            std::cerr << "Unable to reassign variable '" << name << "'\n";
            return;
        }
    }

    void deleteSymbol(std::string name)
    {
        if (table.find(name) != table.end())
        {
            std::cerr << "Unable to delete unknown variable '" << name << "'\n";
            return;
        }
        table.erase(name);
    }
};

class SymbolTableStack
{
public:
    std::vector<SymbolTable> tableStack;

    void enterScope()
    {
        tableStack.push_back(SymbolTable());
    }

    void exitScope()
    {
        tableStack.pop_back();
    }
};