#include "codegenerator.h"
#include <sstream>
#include <string>

std::string generateExpression(ParserNode* node);

std::string generateStatement(ParserNode* node, int indent = 0) {
    std::ostringstream out;
    std::string ind(indent, ' ');

    if (auto decl = dynamic_cast<DeclarationNode*>(node)) {
        if (decl->type == "string")
            out << ind << "std::string " << decl->name;
        else
            out << ind << decl->type << " " << decl->name;
    
        if (decl->value != nullptr)
            out << " = " << generateExpression(decl->value);
        out << ";\n";
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
        out << "\n"; // Single newline after entire if/else block
    }
    else if (auto print = dynamic_cast<PrintNode*>(node)) {
        const std::string& val = print->token.text;
        TokenType type = print->token.type;
        out << ind << "std::cout << ";
        if (type == TokenType::STRING_LITERAL) {
            out << "\"" << val << "\"";
        } else if (type == TokenType::CHAR_LITERAL) {
            out << "'" << val << "'";
        } else {
            out << val;
        }
        out << " << std::endl;\n";
    }
    else if (auto switchNode = dynamic_cast<SwitchNode*>(node)) {
        out << ind << "switch (" << generateExpression(switchNode->condition) << ") {\n";
        for (auto caseNode : switchNode->cases) {
            if (caseNode->value) {
                out << ind << "  case " << generateExpression(caseNode->value) << ":\n";
            } else {
                out << ind << "  default:\n";
            }

            bool hasBreak = false;
            for (auto stmt : caseNode->body) {
                out << generateStatement(stmt, indent + 4);
                if (dynamic_cast<BreakNode*>(stmt)) {
                    hasBreak = true;
                }
            }

            if (!hasBreak) {
                out << ind << "    break;\n";
            }
        }
        out << ind << "}\n";
    }
    else if (auto whileNode = dynamic_cast<WhileLoopNode*>(node)) {
        out << ind << "while (" << generateExpression(whileNode->condition) << ") {\n";
        for (auto stmt : whileNode->statements) {
            out << generateStatement(stmt, indent + 4);
        }
        out << ind << "}\n";
    }
    else if (dynamic_cast<BreakNode*>(node)) {
        out << ind << "break;\n";
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
        if (num->type == TokenType::INTEGER_LITERAL) {
            return std::to_string((int)num->value); // force int
        } else {
            return std::to_string(num->value); // leave float/double as-is
        }
    }
    else if (auto str = dynamic_cast<StringNode*>(node)) {
        return "\"" + str->value + "\"";
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
    out << "#include <iostream>\n";
    out << "#include <string>\n\n";
    out << "int main() {\n";
    out << generateStatement(node, 4);
    out << "    return 0;\n";
    out << "}\n";
    return out.str();
}