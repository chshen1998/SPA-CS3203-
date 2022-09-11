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
Place Holder methods that need to be defined (otherwise theres an error).
Will be removed in the next PR once Hoa Shen's PR is merged in.
*/
set<string> AllVariables() {
    return set<string>();
}
set<string> AllConstants() {
    return set<string>();
}

set<string> AllProcedures() {
    return set<string>();
}

set<string> AllStatements() {
    return set<string>();
}

ostream& operator<<(ostream& os, PqlToken& token) {
    string typeString = "";
    switch (token.type) {
    case(TokenType::VARIABLE): typeString = "variable";
    case(TokenType::CONSTANT): typeString = "constant";
    case(TokenType::ASSIGN): typeString = "assign";
    case(TokenType::PROCEDURE): typeString = "procedure";
    case(TokenType::STATEMENT): typeString = "statement";
    case(TokenType::WHILE): typeString = "while";
    case(TokenType::IF): typeString = "if";
    case(TokenType::ELSE): typeString = "else";
    case(TokenType::PRINT): typeString = "print";
    case(TokenType::CALL): typeString = "call";
    case(TokenType::SYNONYM): typeString = "synonym";
    case(TokenType::NUMBER): typeString = "number";
    case(TokenType::STRING): typeString = "string";
    case(TokenType::SELECT): typeString = "select";
    case(TokenType::PATTERN): typeString = "pattern";
    case(TokenType::USES): typeString = "uses";
    case(TokenType::MODIFIES): typeString = "modifies";
    case(TokenType::PARENT): typeString = "parent";
    case(TokenType::PARENT_A): typeString = "parent_a";
    case(TokenType::FOLLOWS): typeString = "follows";
    case(TokenType::FOLLOWS_A): typeString = "follows_a";
    case(TokenType::SUCH): typeString = "such";
    case(TokenType::THAT): typeString = "that";
    case(TokenType::SEMICOLON): typeString = "semicolon";
    case(TokenType::COMMA): typeString = "comma";
    case(TokenType::OPEN_BRACKET): typeString = "open_bracket";
    case(TokenType::CLOSED_BRACKET): typeString = "closed_bracket";
    case(TokenType::EMPTY): typeString = "empty";
    case(TokenType::DECLARATION_END): typeString = "declaration_end";
    case(TokenType::END): typeString = "end";
    default: typeString = "Unknown";
    }

    os << "[Token: " << typeString << ", Value: " << token.value << "]";
    return os;
}

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
void QPS::evaluate(string query, list<string>& results) {
    
    /* Commented out as we are using mockTokens for now
    QueryTokenizer tokenizer = QueryTokenizer(query);
    vector<PqlToken> tokens = tokenizer.Tokenize();
    */

    QueryExtractor extractor = QueryExtractor(mockTokenize());
    PqlQuery pq = extractor.ExtractSemantics();

    QueryEvaluator evaluator = QueryEvaluator(pq);
    set<string> result = evaluator.CallPKB();

    // string output = evaluator.convertToString(result);
}

