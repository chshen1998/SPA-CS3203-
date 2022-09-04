#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "Parser.h"
#include "QuerySemanticsExtractor.h"
#include "QueryEvaluator.h"
#include "PKB/PKB.h"
#include "AST/TNode.h"

unordered_map<string, TokenType> TokenMap = {
        {"variable", TokenType::VARIABLE},
        {"constant", TokenType::CONSTANT},
        {"assign", TokenType::ASSIGN},
        {"procedure", TokenType::PROCEDURE},
        {"stmt", TokenType::STATEMENT},
        {"while", TokenType::WHILE},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},

        {"Select", TokenType::SELECT},
        {"pattern", TokenType::PATTERN},
        {"Uses", TokenType::USES},
        {"Modifies", TokenType::MODIFIES},
        {"Follows", TokenType::FOLLOWS},
        {"Follows*", TokenType::FOLLOWS_T},
        {"Parent", TokenType::PARENT},
        {"Parent*", TokenType::PARENT_T},

        {"such", TokenType::SUCH},
        {"that", TokenType::THAT},

        {";", TokenType::SEMICOLON},
        {",", TokenType::COMMA},
        {"(", TokenType::OPEN_PARENTHESIS},
        {")", TokenType::CLOSED_PARENTHESIS},
};

unordered_set<TokenType> validDeclarations = {
        TokenType::Variable,
        TokenType::PROCEDURE,
        TokenType::WHILE,
        TokenType::ASSIGN,
        TokenType::STMT
};

unordered_set<TokenType> validSuchThatClauses = {
        TokenType::USES,
        TokenType::MODIFIES,
        TokenType::PARENT,
        TokenType::FOLLOWS
};


int Parser::Parse () {
    QuerySemanticsExtractor extractor = QuerySemanticsExtractor(mockTokenize());
    ParsedQuery pq = extractor.ExtractSemantics();

    QueryEvaluator evaluator = QueryEvaluator(pq);
    vector<string> result = evaluator.CallPKB();
    return 0;
}

