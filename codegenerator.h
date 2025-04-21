#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string>
#include "parser.h"

// Generate C++ code from a parsed syntax tree

std::string generateCode(ParserNode *node);

#endif // CODE_GENERATOR_H