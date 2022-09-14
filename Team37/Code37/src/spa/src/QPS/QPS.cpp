using namespace std;

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <list>

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
        {"print", TokenType::PRINT},
        {"call", TokenType::CALL},

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
 * Set of valid such that clauses
 */
set<TokenType> validPatternParameters = {
        TokenType::SYNONYM,
        TokenType::CONSTANT,
        TokenType::STRING,
        TokenType::NUMBER
};

std::ostream& operator<< (std::ostream& os, const PqlToken& token) {
    string typeString = "Unknown";
    switch (token.type) {
    case(TokenType::VARIABLE): typeString = "variable"; break;
    case(TokenType::CONSTANT):typeString = "constant"; break;
    case(TokenType::ASSIGN): typeString = "assign"; break;
    case(TokenType::PROCEDURE): typeString = "procedure"; break;
    case(TokenType::STATEMENT): typeString = "statement"; break;
    case(TokenType::WHILE): typeString = "while"; break;
    case(TokenType::IF): typeString = "if"; break;
    case(TokenType::ELSE): typeString = "else"; break;
    case(TokenType::PRINT): typeString = "print"; break;
    case(TokenType::CALL): typeString = "call"; break;
    case(TokenType::SYNONYM): typeString = "synonym"; break;
    case(TokenType::NUMBER): typeString = "number"; break;
    case(TokenType::STATEMENT_NUM): typeString = "statement_num"; break;
    case(TokenType::WILDCARD): typeString = "wildcard"; break;
    case(TokenType::WILDCARD_STRING): typeString = "wildcard_string"; break;
    case(TokenType::STRING): typeString = "string"; break;
    case(TokenType::SELECT): typeString = "select"; break;
    case(TokenType::PATTERN): typeString = "pattern"; break;
    case(TokenType::USES): typeString = "uses"; break;
    case(TokenType::MODIFIES): typeString = "modifies"; break;
    case(TokenType::PARENT): typeString = "parent"; break;
    case(TokenType::PARENT_A): typeString = "parent_a"; break;
    case(TokenType::FOLLOWS): typeString = "follows"; break;
    case(TokenType::FOLLOWS_A): typeString = "follows_a"; break;
    case(TokenType::SUCH): typeString = "such"; break;
    case(TokenType::THAT): typeString = "that"; break;
    case(TokenType::SEMICOLON): typeString = "semicolon"; break;
    case(TokenType::COMMA): typeString = "comma"; break;
    case(TokenType::OPEN_BRACKET): typeString = "open_bracket"; break;
    case(TokenType::CLOSED_BRACKET): typeString = "closed_bracket"; break;
    case(TokenType::EMPTY): typeString = "empty"; break;
    case(TokenType::DECLARATION_END): typeString = "declaration_end"; break;
    case(TokenType::END): typeString = "end"; break;
    }

    os << "[ TokenType: " + typeString + " , Value : '" + token.value + "' ]" << endl;

    return os;
}


void QPS::setQueryServicer(shared_ptr<QueryServicer> s) {
    QPS::servicer = s;
}

/*
    * Takes in query string input from user, parses the query string then return result from PKB
    */
void QPS::evaluate(string query, list<string>& results) {
    
    QueryTokenizer tokenizer = QueryTokenizer(query);
    vector<PqlToken> tokens = tokenizer.Tokenize();

    QueryExtractor extractor(tokens);
    PqlQuery pq = extractor.extractSemantics();

    QueryEvaluator evaluator = QueryEvaluator(pq);
    set<string> result = evaluator.CallPKB();
 
    // string output = evaluator.convertToString(result);
}
