using namespace std;

#include <string>
#include <unordered_map>

#include "ErrorType.h"

unordered_map<ErrorType, string> errorTypeToStringMap = {
    { ErrorType::SEMANTIC_ERROR, "SemanticError" },
    { ErrorType::SYNTAX_ERROR, "SyntaxError" }
};
