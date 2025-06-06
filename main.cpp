#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
#include <cctype>
#include "semanticAnalyzer.h" 
#include "tokenizer.h"
#include "parser.h"
#include "codegenerator.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No input file detected!!\n";
        return 1;
    }

    std::ifstream myfile(argv[1]);
    if (!myfile.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return 1;
    }

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
    std::vector<ParserNode *> parserNodes = parser.parse();

    SemanticAnalyzer sem;
    sem.analyze(parserNodes);

    for (ParserNode *node : parserNodes)
    {
        node->print();
        std::cout << std::endl;
    }

    std::cout << "Beginning code generation: \n\n\n";

    // Code generation time!
    std::string inputFilename(argv[1]);

    size_t slash = inputFilename.find_last_of("\\/");
    std::string dir, filename;
    if (slash == std::string::npos)
    {
        dir = "";
        filename = inputFilename;
    }
    else
    {
        dir = inputFilename.substr(0, slash + 1); // e.g. "tests\"
        filename = inputFilename.substr(slash + 1); // e.g. "testcase1.cstar"
    }

    size_t dot = filename.find_last_of('.');
    std::string base = (dot == std::string::npos) ? filename : filename.substr(0, dot);

    // Prepend "_" to the basename, append "_output.cpp", and rejoin
    std::string outputCpp = dir + "_" + base + "_output.cpp";

    std::ofstream outputFile(outputCpp);

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file for writing.\n";
        return 1;
    }

    outputFile << "#include <iostream>\n\n";
    outputFile << "int main() {\n";

    for (ParserNode *node : parserNodes)
    {
        std::string generated = generateCode(node);
        // Strip main() header and footer if included
        size_t bodyStart = generated.find("{");
        size_t bodyEnd = generated.rfind("return 0;");
        if (bodyStart != std::string::npos && bodyEnd != std::string::npos) {
            std::string body = generated.substr(bodyStart + 1, bodyEnd - bodyStart - 1);
            // outputFile << body << "\n";
            outputFile << body;
        } else {
            outputFile << "    // Code generation failed for this node\n";
        }
    }

    outputFile << "return 0;\n";
    outputFile << "}\n";
    outputFile.close();
    std::cout << "C++ code written to: " << outputCpp << std::endl;

    // Compile the generated file
    std::string compileCommand = "g++ " + outputCpp + " -o generated_output";
    int compileStatus = system(compileCommand.c_str());
    if (compileStatus != 0) {
        std::cerr << "Compilation failed!\n";
    } else {
        std::cout << "Running generated program:\n\n";
        std::cout << "\n\n\n\n==================================\n\n\n\n";
        system("generated_output.exe");
        std::cout << "\n\nEnd of file!\n\n==================================\n\n\n\n";
    }

    return 0;
}
