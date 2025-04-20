#include "codegenerator.h"
#include <sstream>

std::string generateExpression(ParserNode* node);

std::string generateStatement(ParserNode* node, int indent = 0) {
    std::ostringstream out;
    std::string ind(indent, ' ');

    if (auto decl = dynamic_cast<DeclarationNode*>(node)) {
        if (decl->type == "string") {
            out << ind << "std::string " << decl->name << ";\n";
        } else {
            out << ind << decl->type << " " << decl->name << ";\n";
        }
    }
    else if (auto assign = dynamic_cast<AssignmentNode*>(node)) {
        out << ind << assign->var->name << " = " << generateExpression(assign->value) << ";\n";
    }
    else if (auto ifNode = dynamic_cast<IfNode*>(node)) {
        out << ind << "if (" << generateExpression(ifNode->condition) << ") {\n";
        for (auto stmt : ifNode->thenBranch) {
            out << generateStatement(stmt, indent + 4);
        }
        out << ind << "}";
        if (!ifNode->elseBranch.empty()) {
            out << " else {\n";
            for (auto stmt : ifNode->elseBranch) {
                out << generateStatement(stmt, indent + 4);
            }
            out << ind << "}";
        }
        out << "\n";
    }
    else if (auto print = dynamic_cast<PrintNode*>(node)) {
        std::string val = print->token.text;
        TokenType type = print->token.type;
    
        if (type == TokenType::STRING_LITERAL) {
            out << ind << "std::cout << \"" << val << "\" << std::endl;\n";
        } else if (type == TokenType::CHAR_LITERAL) {
            out << ind << "std::cout << '" << val << "' << std::endl;\n";
        } else if (type == TokenType::INTEGER_LITERAL ||
                   type == TokenType::FLOAT_LITERAL ||
                   type == TokenType::BOOL_LITERAL ||
                   type == TokenType::IDENTIFIER) {
            out << ind << "std::cout << " << val << " << std::endl;\n";
        } else {
            out << ind << "// unsupported print token: " << val << "\n";
        }
    }
    return out.str();
}

std::string generateExpression(ParserNode* node) {
    if (auto binop = dynamic_cast<BinOpNode*>(node)) {
        return "(" + generateExpression(binop->left) + " " +
               binop->op->getOperatorString() + " " +
               generateExpression(binop->right) + ")";
    }
    else if (auto var = dynamic_cast<VariableNode*>(node)) {
        return var->name;
    }
    else if (auto num = dynamic_cast<NumberNode*>(node)) {
        return std::to_string(num->value);
    }
    else if (auto str = dynamic_cast<StringNode*>(node)) {
        return "\""+ str->value + "\""; // leave as-is for now
    }
    else if (auto chr = dynamic_cast<CharNode*>(node)) {
        return "'" + std::string(1, chr->value) + "'";
    }
    else if (auto boolean = dynamic_cast<BooleanNode*>(node)) {
        return boolean->value ? "true" : "false";
    }
    
    return "";
}

std::string generateCode(ParserNode* node) {
    std::ostringstream out;
    out << "#include <iostream>\n\n";
    out << "int main() {\n";

    if (auto block = dynamic_cast<IfNode*>(node)) {
        out << generateStatement(block, 4);
    }
    else {
        out << generateStatement(node, 4);
    }

    out << "    return 0;\n";
    out << "}\n";
    return out.str();
}