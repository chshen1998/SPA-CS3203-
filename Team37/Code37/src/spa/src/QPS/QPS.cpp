using namespace std;

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "QPS.h"
#include "QuerySemanticsExtractor.h"
#include "QueryEvaluator.h"
#include "AST/TNode.h"
#include "PKB/PKB.h"
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
set<TokenType> validDeclarations = {
        TokenType::VARIABLE,
        TokenType::PROCEDURE,
        TokenType::WHILE,
        TokenType::ASSIGN,
        TokenType::STATEMENT
};

/*
 * Set of valid such that clauses
 */
set<TokenType> validSuchThatClauses = {
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
string QPS::processQuery(string query) {
    
    /*
    * QueryTokenizer tokenizer = QueryTokenizer(query);
    * vector<PqlTokens> tokens = tokenizer.tokenize();
    */
    
    QuerySemanticsExtractor extractor = QuerySemanticsExtractor(mockTokenize());
    PqlQuery pq = extractor.ExtractSemantics();

    QueryEvaluator evaluator = QueryEvaluator(pq);
    set<string> result = evaluator.CallPKB();

    // string output = evaluator.convertToString(result);

    return "";
}

