using namespace std;

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "Parser.h"
#include "QuerySemanticsExtractor.h"
#include "QueryEvaluator.h"
#include "PKB/PKB.h"
#include "AST/TNode.h"
#include <unordered_map>

/*
 * Hashmap containing the tokenType to pql string value mapping
 */
unordered_map<string, TokenType> stringToTokenMap = {
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
        {"Follows*", TokenType::FOLLOWS_A},
        {"Parent", TokenType::PARENT},
        {"Parent*", TokenType::PARENT_A},

        {"such", TokenType::SUCH},
        {"that", TokenType::THAT},

        {";", TokenType::SEMICOLON},
        {",", TokenType::COMMA},
        {"(", TokenType::OPEN_BRACKET},
        {")", TokenType::CLOSED_BRACKET},
};

/*
 * Set of valid declaration types
 */
unordered_set<TokenType> validDeclarations = {
        TokenType::VARIABLE,
        TokenType::PROCEDURE,
        TokenType::WHILE,
        TokenType::ASSIGN,
        TokenType::STATEMENT
};

/*
 * Set of valid such that clauses
 */
unordered_set<TokenType> validSuchThatClauses = {
        TokenType::USES,
        TokenType::MODIFIES,
        TokenType::PARENT,
        TokenType::PARENT_A,
        TokenType::FOLLOWS,
        TokenType::FOLLOWS_A
};

/*
 * Takes in query string input from user, parses the query string then return result from PKB
 */
int Parser::Parse () {
    QuerySemanticsExtractor extractor = QuerySemanticsExtractor(mockTokenize());
    PqlQuery pq = extractor.ExtractSemantics();

    QueryEvaluator evaluator = QueryEvaluator(pq);
    set<string> result = evaluator.CallPKB();
    return 0;
}

