#include <vector>
#include "tokenizer.cpp"

class ParserNode{};

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
    GreaterThan,
    And,
    Or,
    Not
};

class AssignmentNode : public ParserNode
{
public:
    VariableNode *var;
    ParserNode *value;

    AssignmentNode(VariableNode *var, ParserNode *value) : var(var), value(value) {}
};

class IfNode : public ParserNode
{
public:
    ParserNode *condition;
    std::vector<ParserNode*> thenBranch;
    std::vector<ParserNode*> elseBranch;

    IfNode(ParserNode *condition, std::vector<ParserNode*> thenBranch, std::vector<ParserNode*> elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
};

class BooleanNode : public ParserNode 
{
public:
    bool value;
    
    BooleanNode(Token token) 
    {
        value = (token.text == "true");
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
        else if (op == ">")
        {
            type = OperatorType::GreaterThan;
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
            std::cerr << "Invalid Operator\n";
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
};

class NumberNode : public ParserNode
{
public:
    TokenType type;
    double value;

    NumberNode(Token numTok)
    {
        type = numTok.type;
        value = std::stod(numTok.text);
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
};

class Parser
{
private:
    std::vector<Token> &tokens;
    int index = 0;
public:
    Parser(std::vector<Token> &tokens): tokens(tokens) {}   

    ParserNode *parseAssignment()
    {
        if (tokens[index].text != "set")
        {
            std::cerr << "Invalid assignemnt\n";
            return nullptr;
        }
        index++;

        Token varTok = tokens[index];
        if (varTok.type != TokenType::IDENTIFIER)
        {
            std::cerr << "Invalid assignemnt\n";
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
        else
        {
            std::cerr << "Invalid statement\n";
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
        if (tokens[index].text != "else") 
        {
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