using namespace std;

#include <string>
#include <unordered_map>

#include "ErrorType.h"


unordered_map<ErrorType, string> errorTypeToStringMap = {
    {ErrorType::SEMANTIC_ERROR, "Semantic Error"},
    {ErrorType::SYNTAX_ERROR, "Syntax Error"}
};
