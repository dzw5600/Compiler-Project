#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>

// Define categories for tokens
enum class TokenType
{
    KEYWORD,
    DATA_TYPE, // Subset of KEYWORD
    IDENTIFIER, // Variables, function names, etc.
    INTEGER_LITERAL,
    FLOAT_LITERAL, // Includes doubles
    CHAR_LITERAL,
    STRING_LITERAL,
    BOOL_LITERAL,
    OPERATOR,
    PUNCTUATION,
    UNKNOWN,
    END_OF_FILE
};

// Token struct
struct Token
{
    TokenType type;
    std::string text;
    int line;

    std::string typeToString() const;
};

// Tokenizer class
class Tokenizer
{
public:
	Tokenizer(std::istream &inputStream);
	Token getNextToken();
	int getEffectiveLineCount() const;

private:
	std::istream &input;
	std::string currentLine;
	int lineIndex = 0; // Current line number (1-based for reporting)
	int charIndex = 0; // Current character index within currentLine
	bool eofReached = false;
	std::set<int> linesWithTokens; // Keep track of lines that generated tokens

	char peek();
	char consume();
	void skipWhitespaceAndComments();
	Token readIdentifierOrKeyword();
	Token readNumberLiteral();
	Token readStringLiteral();
	Token readCharLiteral();
	Token readOperator();
};

extern const std::set<std::string> KEYWORDS;
extern const std::set<std::string> DATA_TYPES;
extern const std::set<std::string> OPERATORS;
extern const std::set<char> PUNCTUATION;

bool is_operator_char(char c);
bool is_punctuation(char c);

#endif // TOKENIZER_H
