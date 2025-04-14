#include <vector>
#ifndef tokenizer_init
#define tokenizer_init
#include "tokenizer.cpp"
#endif

class VariableNode;

class ParserNode
{
public:
    virtual void print(int indent = 0) 
    {
        for (int i = 0; i < indent; i++)
        {
            std::cout << "  ";
        }
        std::cout << "ParserNode\n";
    }
};

enum OperatorType
{
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulus,
    Increment,
    Decrement,
    Equal,
    NotEqual,
    LessThan,
    LessThanEqualTo,
    GreaterThan,
    GreaterThanEqualTo,
    And,
    Or,
    Not
};

class DeclarationNode : public ParserNode
{
public:
    std::string type;
    std::string name;

    DeclarationNode(std::string type, std::string name)
    {
        this->type = type;
        this->name = name;
    }

    void print(int indent = 0) override 
    {
        for (int i = 0; i < indent; i++) 
        {
            std::cout << "  ";
        }
        std::cout << "Declaration: " << type << " " << name << "\n";
    }
};

class VariableNode : public ParserNode
{
public:
    std::string name;

    VariableNode(Token varTok)
    {
        name = varTok.text;
    }

    void print(int indent = 0) override 
    {
        for (int i = 0; i < indent; i++)
        {
            std::cout << "  ";
        }
        std::cout << "Variable: " << name << "\n";
    }
};

class AssignmentNode : public ParserNode
{
public:
    VariableNode *var;
    ParserNode *value;

    AssignmentNode(VariableNode *var, ParserNode *value) : var(var), value(value) {}

    void print(int indent = 0) override
    {
        for (int i = 0; i < indent; i++)
        {
            std::cout << "  ";
        }
        std::cout << "Assignment:\n";
        var->print(indent + 1);
        value->print(indent + 1);
    }
    
};

class IfNode : public ParserNode
{
public:
    ParserNode *condition;
    std::vector<ParserNode*> thenBranch;
    std::vector<ParserNode*> elseBranch;

    IfNode(ParserNode *condition, std::vector<ParserNode*> thenBranch, std::vector<ParserNode*> elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

    void print(int indent = 0) override
    {
        for (int i = 0; i < indent; i++)
        {
            std::cout << "  ";
        }
        std::cout << "If:\n";
        
        for (int i = 0; i < indent + 1; i++)
        {
            std::cout << "  ";
        }
        std::cout << "Condition:\n";
        condition->print(indent + 2);
    
        for (int i = 0; i < indent + 1; i++)
        {
            std::cout << "  ";
        }
        std::cout << "ThenBranch:\n";
        for (auto stmt : thenBranch)
        {
            stmt->print(indent + 2);
        }
    
        if (!elseBranch.empty()) {
            for (int i = 0; i < indent + 1; i++)
            {
                std::cout << "  ";
            }
            std::cout << "ElseBranch:\n";
            for (auto stmt : elseBranch) {
                stmt->print(indent + 2);
            }
        }
    }  
};

class BooleanNode : public ParserNode 
{
public:
    bool value;
    
    BooleanNode(Token token) 
    {
        value = (token.text == "true");
    }

    void print(int indent = 0) override 
    {
        for (int i = 0; i < indent; i++)
        {
            std::cout << "  ";
        }

        std::cout << "Boolean: " << (value ? "true" : "false") << "\n";
    }
    
};

class OperatorNode : public ParserNode
{
public:
    OperatorType type;
    OperatorNode(Token opTok)
    {
        std::string op = opTok.text;
        
        if (op == "+")
        {
            type = OperatorType::Add;
        }
        else if (op == "-")
        {
            type = OperatorType::Subtract;
        }
        else if (op == "*")
        {
            type = OperatorType::Multiply;
        }
        else if (op == "/")
        {
            type = OperatorType::Divide;
        }
        else if (op == "%")
        {
            type = OperatorType::Modulus;
        }
        else if (op == "++")
        {
            type = OperatorType::Increment;
        }
        else if (op == "--")
        {
            type = OperatorType::Decrement;
        }
        else if (op == "=")
        {
            type = OperatorType::Equal;
        }
        else if (op == "!=")
        {
            type = OperatorType::NotEqual;
        }
        else if (op == "<")
        {
            type = OperatorType::LessThan;
        }
        else if (op == "<=")
        {
            type = OperatorType::LessThanEqualTo;
        }
        else if (op == ">")
        {
            type = OperatorType::GreaterThan;
        }
        else if (op == ">=")
        {
            type = OperatorType::GreaterThanEqualTo;
        }
        else if (op == "&&")
        {
            type = OperatorType::And;
        }
        else if (op == "||")
        {
            type == OperatorType::Or;
        }
        else if (op == "!")
        {
            type == OperatorType::Not;
        }
        else
        {
            std::cerr << "Invalid Operator: " << opTok.text << std::endl;
        }
    }
    std::string getOperatorString()
    {
        if (type == OperatorType::Add)
        {
            return "+";
        }
        else if (type == OperatorType::Subtract)
        {
            return "-";
        }
        else if (type == OperatorType::Multiply)
        {
            return "*";
        }
        else if (type == OperatorType::Divide)
        {
            return "/";
        }
        else if (type == OperatorType::Modulus)
        {
            return "%";
        }
        else if (type == OperatorType::Increment)
        {
            return "++";
        }
        else if (type == OperatorType::Decrement)
        {
            return "--";
        }
        else if (type == OperatorType::Equal)
        {
            return "=";
        }
        else if (type == OperatorType::NotEqual)
        {
            return "!=";
        }
        else if (type == OperatorType::LessThan)
        {
            return "<";
        }
        else if (type == OperatorType::LessThanEqualTo)
        {
            return "<=";
        }
        else if (type == OperatorType::GreaterThan)
        {
            return ">";
        }
        else if (type == OperatorType::GreaterThanEqualTo)
        {
            return ">=";
        }
        else if (type == OperatorType::And)
        {
            return "&&";
        }
        else if (type == OperatorType::Or)
        {
            return "||";
        }
        else if (type == OperatorType::Not)
        {
            return "!";
        }
    }
};

class BinOpNode : public ParserNode
{
public:
    ParserNode *left;
    OperatorNode *op;
    ParserNode *right;
    
    BinOpNode(ParserNode *left, OperatorNode *op, ParserNode *right): left(left), op(op), right(right) {}

    void print(int indent = 0) override
    {
        for (int i = 0; i < indent; i++)
        {
            std::cout << "  ";
        }
        std::cout << "BinaryOperation:\n";
        left->print(indent + 1);
        for (int i = 0; i < indent + 1; i++) 
        {
            std::cout << "  ";
        }
        std::cout << "Operator: " << op->getOperatorString() << "\n";
        right->print(indent + 1);
    }
};

class StringNode : public ParserNode
{
public:
    std::string value;

    StringNode(Token str)
    {
        value = str.text;
    }

    void print(int indent = 0) override 
    {
        for (int i = 0; i < indent; i++)
        {
            std::cout << "  ";
        }
        std::cout << "String: " << value << "\n";
    }
};

class CharNode : public ParserNode
{
public:
    char value;

    CharNode(Token chr)
    {
        value = chr.text[0];
    }

    void print(int indent = 0) override 
    {
        for (int i = 0; i < indent; i++) 
        {
            std::cout << "  ";
        }
        std::cout << "Char: " << value << "\n";
    }
};

class NumberNode : public ParserNode
{
public:
    TokenType type;
    double value;

    NumberNode(Token numTok)
    {
        type = numTok.type;
        if (numTok.text.back() == 'f')
        {
            value = stof(numTok.text);
        }
        else
        {
            value = std::stod(numTok.text);
        }
    }

    void print(int indent = 0) override
    {
        for (int i = 0; i < indent; i++)
        {
            std::cout << "  ";
        }
        std::cout << "Number: " << value << "\n";
    }
};


class Parser
{
private:
    std::vector<Token> &tokens;
    int index = 0;
public:
    Parser(std::vector<Token> &tokens): tokens(tokens) {}   

    std::vector<ParserNode*> parse()
    {
        std::vector<ParserNode*> nodes;
        while (index < tokens.size()-1)
        {
            ParserNode *statement = parseStatement();
            if (statement != nullptr)
            {
                nodes.push_back(statement);
            }
            else
            {
                std::cerr << "Compilation stopped at token index " << index << ": " << tokens[index].text << std::endl;
                break;
            }
        }
        return nodes;
    }

    ParserNode *parseDeclaration()
    {
        std::string type = tokens[index].text;
        index++;

        Token identifier = tokens[index];
        if (identifier.type != TokenType::IDENTIFIER)
        {
            std::cerr << "Expected variable name\n";
            return nullptr;
        }
        std::string varName = identifier.text;
        index++;

        if (tokens[index].text != ";")
        {
            std::cerr << "Expected ';'";
            return nullptr;
        }
        index++;

        return new DeclarationNode(type, varName);
    }

    ParserNode *parseAssignment()
    {
        if (tokens[index].text != "set")
        {
            std::cerr << "Invalid assignment\n";
            return nullptr;
        }
        index++;

        Token varTok = tokens[index];
        if (varTok.type != TokenType::IDENTIFIER)
        {
            std::cerr << "Invalid assignment\n";
            return nullptr;
        }
        VariableNode *var = new VariableNode(varTok);
        index++;

        if (tokens[index].text != "=") 
        {
            std::cerr << "Expected '='\n";
            return nullptr;
        }
        index++;

        ParserNode *value = expression();

        if (tokens[index].text != ";")
        {
            std::cerr << "Expected ';'\n";
            return nullptr;
        }
        index++;

        return new AssignmentNode(var, value);
    }

    ParserNode *parseStatement()
    {
        if (tokens[index].text == "if")
        {
            return parseIf();
        }
        else if (tokens[index].text == "set")
        {
            return parseAssignment();
        }

        else if (tokens[index].type == TokenType::DATA_TYPE) 
        {
            return parseDeclaration();
        }    
        else
        {
            std::cerr << "Invalid statement at token index " << index << ": " << tokens[index].text << std::endl;
            return nullptr;
        }
    }

    std::vector<ParserNode*> parseBlock()
    {
        index++;
        std::vector<ParserNode*> statements;

        while (tokens[index].text != "}")
        {
            statements.push_back(parseStatement());
        }
        index++;

        return statements;
    }

    IfNode *parseIf()
    {
        if (tokens[index].text != "if")
        {
            std::cerr << "Unable to parse if statement\n";
            return nullptr;
        }
        index++;

        if (tokens[index].text != "(")
        {
            std::cerr << "Expected '('\n";
            return nullptr;
        }
        index++;

        ParserNode *condition = logic();

        if (tokens[index].text != ")")
        {
            std::cerr << "Expected ')'\n";
            return nullptr;
        }
        index++;

        std::vector<ParserNode*> thenBranch = parseBlock();
        std::vector<ParserNode*> elseBranch;
        if (tokens[index].text == "else") 
        {
            index++;
            elseBranch = parseBlock();
        }

        return new IfNode(condition, thenBranch, elseBranch);
    }
    
    ParserNode *expression()
    {
        ParserNode *left = term();
        OperatorNode *op; 
        ParserNode *right;
        while (tokens[index].type == TokenType::OPERATOR && (tokens[index].text == "+" || tokens[index].text == "-"))
        {
            op = new OperatorNode(tokens[index]);
            index++;
            right = term();

            left = new BinOpNode(left, op, right);
        }

        return left;
    }

    ParserNode *factor()
    {
        Token currToken = tokens[index];
        if (currToken.type == TokenType::INTEGER_LITERAL || currToken.type == TokenType::FLOAT_LITERAL)
        {
            index++;
            return new NumberNode(currToken);
        }

        else if (currToken.type == TokenType::STRING_LITERAL)
        {
            index++;
            return new StringNode(currToken);
        }

        else if (currToken.type == TokenType::CHAR_LITERAL)
        {
            index++;
            return new CharNode(currToken);
        }

        else if (currToken.type == TokenType::IDENTIFIER)
        {
            index++;
            return new VariableNode(currToken);
        }

        else if (currToken.type == TokenType::BOOL_LITERAL)
        {
            index++;
            return new BooleanNode(currToken);
        }

        else if (currToken.type == TokenType::PUNCTUATION)
        {
            index++;
            ParserNode *node = expression();
            if (tokens[index].type != TokenType::PUNCTUATION)
            {
                std::cerr << "Expected ')'\n";
            }
            index++;
            return node;
        }

        else
        {
            std::cerr << "Syntax Error\n";
            return nullptr;
        }
    }

    ParserNode *term()
    {
        ParserNode *left = factor();
        OperatorNode *op; 
        ParserNode *right;
        while (tokens[index].type == TokenType::OPERATOR && (tokens[index].text == "*" || tokens[index].text == "/"))
        {
            op = new OperatorNode(tokens[index]);
            index++;
            right = factor();

            left = new BinOpNode(left, op, right);
        }

        return left;
    }

    ParserNode *comparison()
    {
        ParserNode *left = expression();
        OperatorNode *op;
        ParserNode *right;

        while (tokens[index].type == TokenType::OPERATOR && 
            (tokens[index].text == "=" || tokens[index].text == "!=" ||
             tokens[index].text == "<"  || tokens[index].text == "<=" ||
             tokens[index].text == ">"  || tokens[index].text == ">=")) 
        {
            op = new OperatorNode(tokens[index]);
            index++;
            right = expression();

            left = new BinOpNode(left, op, right);
        }

        return left;
    }

    ParserNode *logic()
    {
        ParserNode *left = comparison();
        OperatorNode *op; 
        ParserNode *right;
        while (tokens[index].type == TokenType::OPERATOR && (tokens[index].text == "&&" || tokens[index].text == "||"))
        {
            op = new OperatorNode(tokens[index]);
            index++;
            right = comparison();

            left = new BinOpNode(left, op, right);
        }

        return left;
    }
};