#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
#include <cctype>
#ifndef tokenizer_init
#define tokenizer_init
#include "tokenizer.cpp"
#endif
#include "parser.cpp"

int main()
{
  std::ifstream myfile("test.cstar");
  Tokenizer tokenizer(myfile);
  Token token;

  std::vector<Token> tokens;
  std::vector<Token> keywords;
  std::vector<Token> dataTypes;
  std::vector<Token> identifiers;
  std::vector<Token> intLiterals;
  std::vector<Token> floatLiterals;
  std::vector<Token> charLiterals;
  std::vector<Token> stringLiterals;
  std::vector<Token> boolLiterals;
  std::vector<Token> operators;
  std::vector<Token> punctuation;
  std::vector<Token> unknown;


//   std::cout << "\n===== STARTING TOKENIZATION =====\n" << std::endl;

  do
  {
    token = tokenizer.getNextToken();
    tokens.push_back(token);

    // Print tokens as they're found
    // std::cout << "Line " << token.line << ": " << token.typeToString() << " [" << token.text << "]" << std::endl;

    switch (token.type)
    {
    case TokenType::KEYWORD:
      keywords.push_back(token);
      break;
    case TokenType::DATA_TYPE:
      dataTypes.push_back(token);
      break;
    case TokenType::IDENTIFIER:
      identifiers.push_back(token);
      break;
    case TokenType::INTEGER_LITERAL:
      intLiterals.push_back(token);
      break;
    case TokenType::FLOAT_LITERAL:
      floatLiterals.push_back(token);
      break;
    case TokenType::CHAR_LITERAL:
      charLiterals.push_back(token);
      break;
    case TokenType::STRING_LITERAL:
      stringLiterals.push_back(token);
      break;
    case TokenType::BOOL_LITERAL:
      boolLiterals.push_back(token);
      break;
    case TokenType::OPERATOR:
      operators.push_back(token);
      break;
    case TokenType::PUNCTUATION:
      punctuation.push_back(token);
      break;
    case TokenType::UNKNOWN:
      if (!token.text.empty())
      {
        unknown.push_back(token);
      }
      break;
    case TokenType::END_OF_FILE:
      break;
    }
  } while (token.type != TokenType::END_OF_FILE);

//   std::cout << "\n===== TOKENIZATION REPORT =====\n" << std::endl;

//   std::cout << "Nonempty lines processed: " << tokenizer.getEffectiveLineCount() << std::endl;

  auto printCategory = [](const std::string &categoryName, const std::vector<Token> &tokens)
  {
    // std::cout << "\n" << categoryName << " (" << tokens.size() << "):" << std::endl;
    std::set<std::string> uniqueTokens; // Show unique tokens found

    for (const auto &t : tokens)
    {
      uniqueTokens.insert(t.text);
    }

    if (uniqueTokens.empty())
    {
    //   std::cout << "  (None)" << std::endl;
    }
    else
    {
      int count = 0;
      for (const auto &text : uniqueTokens)
      {
        // std::cout << "  " << text << (++count % 5 == 0 ? "\n" : "");
      }
    //   if (count % 5 != 0) std::cout << std::endl; // Ensure newline at end
    }
  };

//   printCategory("Data Types", dataTypes);
//   printCategory("Other Keywords", keywords); // (Keywords not classified as types/bools)
//   printCategory("Operators", operators);
//   printCategory("Identifiers (Variables/Functions/etc.)", identifiers);
//   printCategory("Integer Literals", intLiterals);
//   printCategory("Float/Double Literals", floatLiterals);
//   printCategory("Boolean Literals", boolLiterals);
//   printCategory("Character Literals", charLiterals);
//   printCategory("String Literals", stringLiterals);
//   printCategory("Punctuation", punctuation);
//   printCategory("Unknown Tokens", unknown);

  Parser parser(tokens);
  std::vector<ParserNode*> parsernodes = parser.parse();

  for (ParserNode *node : parsernodes)
  {
    node->print();
  }

  return 0;
}
