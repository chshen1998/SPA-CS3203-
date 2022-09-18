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
#include "QueryValidator.h"
#include "./Validators/ValidatorUtils.h"
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
		{"read", TokenType::READ},

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

unordered_map<ErrorType, string> errorTypeToStringMap = {
    {ErrorType::SEMANTIC_ERROR, "Semantic Error"},
    {ErrorType::SYNTAX_ERROR, "Syntax Error"}
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
    vector<PqlToken> tokens;
    try
    {
	    tokens = tokenizer.Tokenize();
    } catch (SyntaxError e)
    {
        results.push_back("Syntax Error");
        results.push_back(e.message);
        return;
    } 
    

    QueryValidator validator = QueryValidator(tokens);
    PqlError pe = validator.validateQuery();

    if (pe.errorType != ErrorType::NONE)
    {
        results.push_back(errorTypeToStringMap[pe.errorType]);
        results.push_back(pe.message);
        return;
    }

    QueryExtractor extractor(tokens);
    PqlQuery pq = extractor.extractSemantics();


    QueryEvaluator evaluator = QueryEvaluator(pq, servicer, results);
    evaluator.evaluate();

}
