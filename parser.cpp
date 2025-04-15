#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include "parser.h"

bool DEBUG_PARSING = true;

// Add this helper function for indented debug output
void debugPrint(const std::string& message, int depth = 0)
{
    if (!DEBUG_PARSING) return;
    std::cout << std::string(depth * 2, ' ') << "DEBUG: " << message << std::endl;
}

void ParserNode::print(int indent)
{
    for (int i = 0; i < indent; i++)
    {
        std::cout << "  ";
    }
    std::cout << "ParserNode\n";
}

DeclarationNode::DeclarationNode(std::string type, std::string name)
{
    this->type = type;
    this->name = name;
}

void DeclarationNode::print(int indent) 
{
    for (int i = 0; i < indent; i++)
    {
        std::cout << "  ";
    }
    std::cout << "Declaration: " << type << " " << name << "\n";
}

VariableNode::VariableNode(Token varTok)
{
    name = varTok.text;
}

void VariableNode::print(int indent) 
{
    for (int i = 0; i < indent; i++)
    {
        std::cout << "  ";
    }
    std::cout << "Variable: " << name << "\n";
}

AssignmentNode::AssignmentNode(VariableNode *var, ParserNode *value) : var(var), value(value)
{

}

void AssignmentNode::print(int indent)
{
    for (int i = 0; i < indent; i++)
    {
        std::cout << "  ";
    }
    std::cout << "Assignment:\n";
    var->print(indent + 1);
    value->print(indent + 1);
}

IfNode::IfNode(ParserNode *condition, std::vector<ParserNode*> thenBranch, std::vector<ParserNode*> elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch)
{

}

void IfNode::print(int indent)
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

BooleanNode::BooleanNode(Token token) 
{
    value = (token.text == "true");
}

void BooleanNode::print(int indent) 
{
    for (int i = 0; i < indent; i++)
    {
        std::cout << "  ";
    }

    std::cout << "Boolean: " << (value ? "true" : "false") << "\n";
}

OperatorNode::OperatorNode(Token opTok)
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
        type = OperatorType::Or;
    }
    else if (op == "!")
    {
        type = OperatorType::Not;
    }
    else
    {
        std::cerr << "Invalid Operator: " << opTok.text << std::endl;
    }
}

std::string OperatorNode::getOperatorString()
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
    
    // Base case (not sure if this should ever be called)
    return "";
}

BinOpNode::BinOpNode(ParserNode *left, OperatorNode *op, ParserNode *right): left(left), op(op), right(right)
{

}

void BinOpNode::print(int indent)
{
    for (int i = 0; i < indent; i++)
        std::cout << "  ";

    std::cout << "BinaryOperation:\n";
    left->print(indent + 1);

    for (int i = 0; i < indent + 1; i++) 
        std::cout << "  ";

    std::cout << "Operator: " << op->getOperatorString() << "\n";
    right->print(indent + 1);
}

StringNode::StringNode(Token str)
{
    value = str.text;
}

void StringNode::print(int indent) 
{
    for (int i = 0; i < indent; i++)
        std::cout << "  ";

    std::cout << "String: " << value << "\n";
}

CharNode::CharNode(Token chr)
{
    value = chr.text[0];
}

void CharNode::print(int indent) 
{
    for (int i = 0; i < indent; i++) 
        std::cout << "  ";

    std::cout << "Char: " << value << "\n";
}

NumberNode::NumberNode(Token numTok)
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

void NumberNode::print(int indent)
{
    for (int i = 0; i < indent; i++)
        std::cout << "  ";

    std::cout << "Number: " << value << "\n";
}


Parser::Parser(std::vector<Token> &tokens): tokens(tokens), index(0)
{
    
}  

std::vector<ParserNode*> Parser::parse()
{
    debugPrint("Starting parsing");

    std::vector<ParserNode*> nodes;
    while (index < tokens.size() - 1)
    {
        debugPrint("Parsing statement at token: " + tokens[index].text, 1);
        ParserNode *statement = parseStatement();
        if (statement != nullptr)
        {
            debugPrint("Successfully parsed statement", 1);
            nodes.push_back(statement);
        }
        else
        {
            std::cerr << "Compilation stopped at token index " << index << ": " << tokens[index].text << std::endl;
            break;
        }
    }

    debugPrint("Parsing complete, found " + std::to_string(nodes.size()) + " statements");
    return nodes;
}

ParserNode* Parser::parseDeclaration()
{
    debugPrint("Parsing declaration", 1);

    std::string type = tokens[index].text;
    debugPrint("Type: " + type, 2);
    index++;

    Token identifier = tokens[index];
    if (identifier.type != TokenType::IDENTIFIER)
    {
        std::cerr << "Expected variable name\n";
        return nullptr;
    }
    std::string varName = identifier.text;
    debugPrint("Variable name: " + varName, 2);
    index++;

    if (tokens[index].text != ";")
    {
        std::cerr << "Expected ';'";
        return nullptr;
    }
    index++;

    debugPrint("Created declaration node: " + type + " " + varName, 2);
    return new DeclarationNode(type, varName);
}

ParserNode* Parser::parseAssignment()
{
    debugPrint("Parsing assignment", 1);

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

    debugPrint("Variable to assign: " + varTok.text, 2);
    VariableNode *var = new VariableNode(varTok);
    index++;

    if (tokens[index].text != "=") 
    {
        std::cerr << "Expected '='\n";
        return nullptr;
    }
    index++;

    debugPrint("Parsing expression for assignment", 2);
    ParserNode *value = expression();
    debugPrint("Expression parsed", 2);

    if (tokens[index].text != ";")
    {
        std::cerr << "Expected ';'\n";
        return nullptr;
    }
    index++;

    debugPrint("Created assignment node", 2);
    return new AssignmentNode(var, value);
}

ParserNode* Parser::parseStatement()
{
    debugPrint("Determining statement type", 1);
    if (tokens[index].text == "if")
    {
        debugPrint("Found if statement", 2);
        return parseIf();
    }
    else if (tokens[index].text == "set")
    {
        debugPrint("Found assignment statement", 2);
        return parseAssignment();
    }
    else if (tokens[index].type == TokenType::DATA_TYPE)
    {
        debugPrint("Found variable declaration", 2);
        return parseDeclaration();
    }    
    else
    {
        std::cerr << "Invalid statement at token index " << index << ": " << tokens[index].text << std::endl;
        return nullptr;
    }
}

std::vector<ParserNode*> Parser::parseBlock()
{
    debugPrint("Parsing code block", 1);
    index++;
    std::vector<ParserNode*> statements;

    debugPrint("Parsing statements in block", 2);
    while (tokens[index].text != "}")
    {
        statements.push_back(parseStatement());
    }
    index++;

    debugPrint("Block complete with " + std::to_string(statements.size()) + " statements", 1);
    return statements;
}

IfNode* Parser::parseIf()
{
    debugPrint("Parsing if statement", 1);

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

    debugPrint("Parsing condition", 2);
    ParserNode *condition = logic();
    debugPrint("Condition parsed", 2);

    if (tokens[index].text != ")")
    {
        std::cerr << "Expected ')'\n";
        return nullptr;
    }
    index++;

    debugPrint("Parsing 'then' branch", 2);
    std::vector<ParserNode*> thenBranch = parseBlock();
    std::vector<ParserNode*> elseBranch;

    if (tokens[index].text == "else") 
    {
        debugPrint("Parsing 'else' branch", 2);
        index++;
        elseBranch = parseBlock();
    }

    debugPrint("If statement complete", 1);
    return new IfNode(condition, thenBranch, elseBranch);
}

ParserNode* Parser::expression()
{
    debugPrint("Parsing expression", 2);
    ParserNode *left = term();
    OperatorNode *op;
    ParserNode *right;

    while (tokens[index].type == TokenType::OPERATOR && (tokens[index].text == "+" || tokens[index].text == "-"))
    {
        debugPrint("Found operator: " + tokens[index].text, 3);
        op = new OperatorNode(tokens[index]);
        index++;
        debugPrint("Parsing right side of binary operator", 3);
        right = term();

        left = new BinOpNode(left, op, right);
        debugPrint("Created binary operation node", 3);
    }

    return left;
}

ParserNode* Parser::factor()
{
    debugPrint("Parsing factor", 2);
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

ParserNode* Parser::term()
{
    debugPrint("Parsing term", 2);

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

ParserNode* Parser::comparison()
{
    debugPrint("Parsing comparison", 2);

    ParserNode *left = expression();
    OperatorNode *op;
    ParserNode *right;

    while (tokens[index].type == TokenType::OPERATOR &&
        (
            tokens[index].text == "=" ||
            tokens[index].text == "!=" ||
            tokens[index].text == "<"  ||
            tokens[index].text == "<=" ||
            tokens[index].text == ">"  ||
            tokens[index].text == ">="
        )
    ) 
    {
        op = new OperatorNode(tokens[index]);
        index++;
        right = expression();

        left = new BinOpNode(left, op, right);
    }

    return left;
}

ParserNode* Parser::logic()
{
    debugPrint("Parsing logic", 2);

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
