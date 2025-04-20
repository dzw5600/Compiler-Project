#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <iostream>
#include "tokenizer.h"

// Forward declaration
class VariableNode;

// Base AST class
class ParserNode
{
public:
	virtual void print(int indent = 0);
	virtual ~ParserNode() = default;
};

// Operator types
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

class WhileLoopNode : public ParserNode
{
public:
	ParserNode *condition;
	std::vector<ParserNode*> statements;

	WhileLoopNode(ParserNode *condition, std::vector<ParserNode*> statements);
	void print(int indent = 0) override;
};

class FunctionDeclaration : public ParserNode
{
public:
	std::string name;
	std::vector<std::string> parameters;
	std::vector<ParserNode*> body;

	FunctionDeclaration(std::string name, std::vector<std::string> parameters, std::vector<ParserNode*> body);
	void print(int indent = 0) override;
};

class FunctionCall : public ParserNode
{
public:
	std::string name;
	std::vector<ParserNode*> arguments;

	FunctionCall(std::string name, std::vector<ParserNode*> arguments);
	void print(int indent = 0) override;
};

class DeclarationNode : public ParserNode
{
public:
	std::string type;
	std::string name;
	ParserNode* value = nullptr; // ✅ optional

	DeclarationNode(std::string type, std::string name, ParserNode* value = nullptr);
	void print(int indent = 0) override;
};

class VariableNode : public ParserNode
{
public:
	std::string name;

	VariableNode(Token varTok);
	void print(int indent = 0) override;
};

class AssignmentNode : public ParserNode
{
public:
	VariableNode *var;
	ParserNode *value;

	AssignmentNode(VariableNode *var, ParserNode *value);
	void print(int indent = 0) override;
};

class IfNode : public ParserNode
{
public:
	ParserNode *condition;
	std::vector<ParserNode*> thenBranch;
	std::vector<ParserNode*> elseBranch;

	IfNode(ParserNode *condition, std::vector<ParserNode*> thenBranch, std::vector<ParserNode*> elseBranch);
	void print(int indent = 0) override;
};

class PrintNode : public ParserNode
{
public:
	Token token;

	PrintNode(Token token);
	void print(int indent = 0) override;
};

class BooleanNode : public ParserNode
{
public:
	bool value;

	BooleanNode(Token token);
	void print(int indent = 0) override;
};

class OperatorNode : public ParserNode
{
public:
	OperatorType type;
	OperatorNode(Token opTok);
	std::string getOperatorString();
};

class BinOpNode : public ParserNode
{
public:
	ParserNode *left;
	OperatorNode *op;
	ParserNode *right;

	BinOpNode(ParserNode *left, OperatorNode *op, ParserNode *right);
	void print(int indent = 0) override;
};

class StringNode : public ParserNode
{
public:
	std::string value;

	StringNode(Token str);
	void print(int indent = 0) override;
};

class CharNode : public ParserNode
{
public:
	char value;

	CharNode(Token chr);
	void print(int indent = 0) override;
};

class NumberNode : public ParserNode
{
public:
	TokenType type;
	double value;

	NumberNode(Token numTok);
	void print(int indent = 0) override;
};

// ====== Switch/Case AST Support ======
class CaseNode : public ParserNode
{
public:
	ParserNode* value; // nullptr for default
	std::vector<ParserNode*> body;

	CaseNode(ParserNode* value, std::vector<ParserNode*> body);
	void print(int indent = 0) override;
};

class SwitchNode : public ParserNode
{
public:
	ParserNode* condition;
	std::vector<CaseNode*> cases;

	SwitchNode(ParserNode* condition, std::vector<CaseNode*> cases);
	void print(int indent = 0) override;
};

// ====== Break Node Support ======
class BreakNode : public ParserNode
{
public:
	void print(int indent = 0) override;
};
// Parser class
class Parser
{
private:
	std::vector<Token> &tokens;
	int index;
public:
	Parser(std::vector<Token> &tokens);

	std::vector<ParserNode*> parse();
	ParserNode *parseDeclaration();
	ParserNode *parseAssignment();
	ParserNode *parsePrint();
	ParserNode *parseStatement();
	std::vector<ParserNode*> parseBlock();
	IfNode *parseIf();
	ParserNode *expression();
	ParserNode *factor();
	ParserNode *term();
	ParserNode *comparison();
	ParserNode *logic();
	ParserNode *parseFunctionDeclaration();
	ParserNode *parseWhileLoop();
	ParserNode *parseSwitch(); // switch/case support
};

#endif // PARSER_H