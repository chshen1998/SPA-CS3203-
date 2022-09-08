using namespace std;

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "QPS.h"
#include "QueryExtractor.h"
#include "QueryEvaluator.h"
#include "QueryTokenizer.h"
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
Hard-coded for the demo for now.
*/
vector<PqlToken> mockTokenize() {
    vector<PqlToken> v;

    v.push_back(PqlToken(TokenType::VARIABLE, "variable"));
    v.push_back(PqlToken(TokenType::SYNONYM, "v"));
    v.push_back(PqlToken(TokenType::SEMICOLON, ";"));
    v.push_back(PqlToken(TokenType::SELECT, "Select"));
    v.push_back(PqlToken(TokenType::SYNONYM, "v"));

    return v;
}


/*
 * Takes in query string input from user, parses the query string then return result from PKB
 */
vector<string> QPS::processQuery(string query) {
    
    /* Commented out as we are using mockTokens for now
    QueryTokenizer tokenizer = QueryTokenizer(query);
    vector<PqlToken> tokens = tokenizer.Tokenize();
    */

    QueryExtractor extractor = QueryExtractor(mockTokenize());
    PqlQuery pq = extractor.ExtractSemantics();

    QueryEvaluator evaluator = QueryEvaluator(pq);
    set<string> result = evaluator.CallPKB();

    // string output = evaluator.convertToString(result);

    vector<string> s;
    return s;
}

