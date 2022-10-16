
#include "TokenizerUtils.h"
#include "../Types/TokenType.h"
#include <unordered_map>

unordered_map<TokenType, ClauseType> TokenizerUtils::tokenTypeToClauseTypeMap =
{
    {TokenType::USES, ClauseType::USE},
    {TokenType::MODIFIES, ClauseType::MODIFIES},
    {TokenType::CALLS, ClauseType::CALLS},
    {TokenType::WHILE, ClauseType::WHILE},
    {TokenType::IF, ClauseType::IF }
};