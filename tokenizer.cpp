#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
#include <cctype>
#include "tokenizer.h"
#include "parser.h"

// Lexical Specification Data

const std::set<std::string> KEYWORDS = {
    "if", "elif", "else", "switch", "case", "default",
    "while", "for", "break", "continue", "return", "set",
    "class", "struct", "interface", "private", "protected",
    "public", "static", "true", "false", "const",
    "int", "float", "double", "char", "bool", "void",
    "string", "new", "base", "extends","print"};

const std::set<std::string> DATA_TYPES = {"int", "float", "double", "char", "bool", "void", "string"};

// Note: Ambiguity of '=' handled by listing it here. 'set' is treated as keyword.
const std::set<std::string> OPERATORS = {
    "+",
    "-",
    "*",
    "/",
    "%",
    "++",
    "--",
    "=",
    "!=",
    "<",
    ">",
    "<=",
    ">=",
    "&&",
    "||",
    "!",
    ".",
};

const std::set<char> PUNCTUATION = {';', '(', ')', '{', '}', ',', ':'};

// Helper Functions

bool is_operator_char(char c)
{
    // Check for characters that might start an operator
    return std::string("+-*/%<>=!&|.").find(c) != std::string::npos;
}

bool is_punctuation(char c) { return PUNCTUATION.count(c); }

// Token methods
std::string Token::typeToString() const
{
    switch (type)
    {
    case TokenType::KEYWORD:
        return "Keyword";
    case TokenType::DATA_TYPE:
        return "Data Type";
    case TokenType::IDENTIFIER:
        return "Identifier (Variable/Other)";
    case TokenType::INTEGER_LITERAL:
        return "Literal (Integer)";
    case TokenType::FLOAT_LITERAL:
        return "Literal (Float/Double)";
    case TokenType::CHAR_LITERAL:
        return "Literal (Char)";
    case TokenType::STRING_LITERAL:
        return "Literal (String)";
    case TokenType::BOOL_LITERAL:
        return "Literal (Boolean)";
    case TokenType::OPERATOR:
        return "Operator";
    case TokenType::PUNCTUATION:
        return "Punctuation";
    case TokenType::UNKNOWN:
        return "Unknown";
    case TokenType::END_OF_FILE:
        return "EOF";
    default:
        return "ErrorType";
    }
}

// Tokenizer

Tokenizer::Tokenizer(std::istream &inputStream) : input(inputStream), lineIndex(0), charIndex(0), eofReached(false)
{
}

// Get next token from stream
Token Tokenizer::getNextToken()
{
    skipWhitespaceAndComments();

    char c = peek();
    int currentTokenLine = lineIndex; // Line num before consuming

    if (c == '\0')
    {
        return {TokenType::END_OF_FILE, "", currentTokenLine};
    }

    Token resultToken;

    // (Order of checks matters here)
    if (is_punctuation(c))
    {
        resultToken = {TokenType::PUNCTUATION, std::string(1, consume()), currentTokenLine};
    }
    else if (c == '"')
    {
        resultToken = readStringLiteral();
    }
    else if (c == '\'')
    {
        resultToken = readCharLiteral();
    }
    else if (std::isdigit(c))
    {
        resultToken = readNumberLiteral();
    }
    else if (c == '.' && charIndex + 1 < currentLine.length() && std::isdigit(currentLine[charIndex + 1]))
    {
        resultToken = readNumberLiteral();
    }
    else if (std::isalpha(c) || c == '_')
    {
        resultToken = readIdentifierOrKeyword();
    }
    else if (is_operator_char(c))
    {
        resultToken = readOperator();
    }
    else
    {
        // If nothing else matched . . .
        resultToken = {TokenType::UNKNOWN, std::string(1, consume()), currentTokenLine};
    }

    // If nonempty token generated, record line num
    if (resultToken.type != TokenType::END_OF_FILE && resultToken.type != TokenType::UNKNOWN)
    {
        linesWithTokens.insert(resultToken.line);
    }
    else if (resultToken.type == TokenType::UNKNOWN && !resultToken.text.empty())
    {
        linesWithTokens.insert(resultToken.line); // Count line even if token unknown to keep track
    }

    return resultToken;
}

// Get the count of lines that contained actual tokens
int Tokenizer::getEffectiveLineCount() const
{
    return linesWithTokens.size();
}

// Get next char, handling line breaks
char Tokenizer::peek()
{
    if (eofReached)
        return '\0';

    while (charIndex >= currentLine.length())
    {
        if (!std::getline(input, currentLine))
        {
            // If the very last line had content ending without newline, lineIndex might be right; fine if empty
            eofReached = true;
            return '\0';
        }

        lineIndex++;
        charIndex = 0;
        currentLine += '\n'; // Add newline to handle it like whitespace
    }
    return currentLine[charIndex];
}

// Consumes char + increments charIndex
char Tokenizer::consume()
{
    char c = peek();
    if (c != '\0')
    {
        charIndex++;
    }
    return c;
}

// Skips whitespace + comments
void Tokenizer::skipWhitespaceAndComments()
{
    while (true)
    {
        char c = peek();
        if (c == '\0')
            return; // End of file

        // Whitespace
        if (std::isspace(c))
        {
            consume();
            continue;
        }

        // Single-line comment
        if (c == '/' && charIndex + 1 < currentLine.length() && currentLine[charIndex + 1] == '/')
        {
            while (peek() != '\n' && peek() != '\0')
            {
                consume();
            }
            continue; // Skip newline itself in next iteration
        }

        // Multi-line comment
        if (c == '/' && charIndex + 1 < currentLine.length() && currentLine[charIndex + 1] == '*')
        {
            consume(); // Consume '/'
            consume(); // Consume '*'
            while (true)
            {
                char next = consume();
                if (next == '\0')
                {
                    std::cerr << "Warning: Unterminated multi-line comment at line " << lineIndex << std::endl;
                    eofReached = true; // Basically treat as end of file
                    return;
                }
                if (next == '*' && peek() == '/')
                {
                    consume(); // Consume '/'
                    break;     // End of comment
                }
            }
            continue;
        }

        // If not whitespace or comment start -> break
        break;
    }
}

// Reads an identifier or keyword
Token Tokenizer::readIdentifierOrKeyword()
{
    std::string text;
    int startLine = lineIndex;
    while (true)
    {
        char c = peek();
        if (c == '\0' || (!std::isalnum(c) && c != '_'))
        {
            break;
        }
        text += consume();
    }

    if (KEYWORDS.count(text))
    {
        if (DATA_TYPES.count(text))
        {
            return {TokenType::DATA_TYPE, text, startLine};
        }
        if (text == "true" || text == "false")
        {
            return {TokenType::BOOL_LITERAL, text, startLine};
        }
        return {TokenType::KEYWORD, text, startLine};
    }

    // Validate identifier format (basic check, must start with letter or underscore)
    if (!text.empty() && (std::isalpha(text[0]) || text[0] == '_'))
    {
        return {TokenType::IDENTIFIER, text, startLine};
    }

    // Doesn't match keyword/identifier rules
    return {TokenType::UNKNOWN, text, startLine};
}

// Reads a number literal (integer/float/double)
Token Tokenizer::readNumberLiteral()
{
    std::string text;
    int startLine = lineIndex;
    bool hasDecimal = false;
    bool isFloat = false; // '.', 'f' seen

    if (peek() == '.')
    {
        // Already confirmed in getNextToken that this '.' is followed by a digit
        hasDecimal = true;
        isFloat = true;
        text += consume(); // Consume the leading '.'
    }

    // Now consume digits and potentially 1 more '.' (which shouldn't happen if started with '.')
    while (true)
    {
        char c = peek();
        if (std::isdigit(c))
        {
            text += consume();
        }
        else if (c == '.' && !hasDecimal)
        {
            hasDecimal = true;
            isFloat = true;
            text += consume();
        }
        else if (c == 'f' && !text.empty() && text != ".")
        {
            isFloat = true;
            text += consume();
            break; // Assume 'f' ends the float literal
        }
        else
        {
            break; // End of number
        }
    }

    // Determine type based on whether a decimal point or 'f' was encountered
    return {isFloat ? TokenType::FLOAT_LITERAL : TokenType::INTEGER_LITERAL, text, startLine};
}

// Reads a string literal
Token Tokenizer::readStringLiteral()
{
    std::string text;
    int startLine = lineIndex;
    consume(); // Consume opening '"'
    while (peek() != '"')
    {
        char c = consume();
        if (c == '\0' || c == '\n')
        {
            // Check for unterminated string
            std::cerr << "Warning: Unterminated string literal at line " << startLine << std::endl;
            return {TokenType::UNKNOWN, text, startLine};
        }
        // (Basic ver: doesn't handle escape sequences like \")
        text += c;
    }
    consume(); // Consume closing '"'
    return {TokenType::STRING_LITERAL, text, startLine};
}

// Reads a char literal
Token Tokenizer::readCharLiteral()
{
    std::string text;
    int startLine = lineIndex;
    consume();            // Consume opening '''
    char val = consume(); // Get the character
    if (val == '\0' || val == '\n')
    {
        std::cerr << "Warning: Unterminated char literal at line " << startLine << std::endl;
        return {TokenType::UNKNOWN, "", startLine};
    }

    // (Basic ver: doesn't handle escape sequences like \')
    text += val;
    if (peek() != '\'')
    {
        std::cerr << "Warning: Multi-character char literal or unterminated char literal at line " << startLine << std::endl;
        // Consume until ' or newline/EOF for basic recovery
        while (peek() != '\'' && peek() != '\n' && peek() != '\0')
            consume();
        if (peek() == '\'')
            consume(); // Consume closing quote if found
        return {TokenType::UNKNOWN, text, startLine};
    }

    consume(); // Consume closing '''
    return {TokenType::CHAR_LITERAL, text, startLine};
}

// Reads an operator
Token Tokenizer::readOperator()
{
    std::string text;
    int startLine = lineIndex;
    char c1 = consume();
    text += c1;

    // Check for two-character operators
    char c2 = peek();
    std::string twoCharOp = text + c2;
    if (OPERATORS.count(twoCharOp))
    {
        text += consume(); // Consume second char
    }
    else if (!OPERATORS.count(text))
    {
        // If the single char wasn't an operator either (rare case!)
        return {TokenType::UNKNOWN, text, startLine};
    }

    return {TokenType::OPERATOR, text, startLine};
}
