using namespace std;

#include <string>
#include <unordered_map>

#ifndef TEAM37_ERRORTYPE_H
#define TEAM37_ERRORTYPE_H

enum class ErrorType {
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    NONE
};

extern unordered_map<ErrorType, string> errorTypeToStringMap;

#endif