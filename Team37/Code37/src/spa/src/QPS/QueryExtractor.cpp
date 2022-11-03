#include <iso646.h>

using namespace std;

#include <string>
#include <vector>
#include <set>
#include <memory>

#include "QueryExtractor.h"
#include "./Extractors/BaseExtractor.h"
#include "./Extractors/DeclarationExtractor.h"
#include "./Extractors/SelectExtractor.h"
#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include <iostream>
#include "./Types/TokenType.h"
#include "Validators/ValidatorUtils.h"

set<char> validOperators = {
    '+', '*', '-', '/'
};

QueryExtractor::QueryExtractor(vector<PqlToken> *tokenVector, shared_ptr<PqlQuery> pq_pointer)
{
    tokens = tokenVector;
    size = tokens->size();
    next = 0;
    booleanIsSynonym = false;
    pq = pq_pointer;
 
}

void QueryExtractor::extractSemantics()
{
    extractDeclarations();
    extractSelect();
    extractClauses();
}

void QueryExtractor::extractDeclarations()
{   
    int start = next;
    while (tokens->at(next).type != TokenType::DECLARATION_END) {
        next += 1;
    }

    shared_ptr<BaseExtractor> extractor = shared_ptr<BaseExtractor>(new DeclarationExtractor(pq, tokens));
    extractor->extract(start, next);
    next += 1;
}

void QueryExtractor::extractSelect()
{
    int start = next;
    while (tokens->at(next).type != TokenType::PATTERN && tokens->at(next).type != TokenType::SUCH && tokens->at(next).type != TokenType::WITH) {
        next += 1;
    }

    shared_ptr<BaseExtractor> extractor = shared_ptr<BaseExtractor>(new SelectExtractor(pq, tokens));
    extractor->extract(start, next);
}

void QueryExtractor::extractClauses()
{   
    PqlToken nextToken = tokens->at(next);
    next += 1;
    pq->clauses.push_back(vector<Clause>{});
    if (pq->clauses.size() == 0) {
        throw SyntaxError("");
    }

    while (nextToken.type != TokenType::END)
    {
        switch (nextToken.type) {
        case TokenType::PATTERN:
            nextToken = extractPatternClause();
            break;
        case TokenType::WITH:
            nextToken = extractWithClause();
            break;
        default:
            nextToken = extractSuchThatClause();
            break;
        }
    }
}

PqlToken QueryExtractor::extractPatternClause()
{
    PqlToken pattern;
    PqlToken left;
    PqlToken right;

    PqlToken next = PqlToken(TokenType::AND, "and");
    while (next.type == TokenType::AND)
    {
        pattern = getNextToken();
        getNextToken(); // OPEN BRACKET
        left = extractString(getNextToken());
        getNextToken(); // COMMA
        right = extractString(getNextToken());
        getNextToken(); // CLOSE BRACKET or COMMA for IF pattern

        // For WHILE and IF pattern, only left arg matters since mid/right args must be wildcard 
        pq->clauses[0].push_back(Clause(pattern, left, right, TokenType::PATTERN));

        if (pq->declarations[pattern.value] == TokenType::IF) {
            getNextToken(); // right arg
            getNextToken(); // close bracket
        }

        next = getNextToken();
    }
    return next;
}

PqlToken QueryExtractor::extractWithClause()
{
    PqlToken left;
    PqlToken leftAttr;
    PqlToken right;
    PqlToken rightAttr;

    PqlToken next = PqlToken(TokenType::AND, "and");
    while (next.type == TokenType::AND)
    {
        left = extractString(getNextToken());
        leftAttr = PqlToken(TokenType::NONE, "");

        next = getNextToken(); // Either "." or "="
        if (next.type == TokenType::DOT)
        {
            leftAttr = getNextToken();
            getNextToken(); // "="
        }

        right = extractString(getNextToken());
        rightAttr = PqlToken(TokenType::NONE, "");

        next = getNextToken(); // Either "." or "and"
        if (next.type == TokenType::DOT)
        {
            rightAttr = getNextToken();
            next = getNextToken(); // Either "and" or next clause type
        }

        pq->clauses[0].push_back(Clause(PqlToken(TokenType::NONE, ""), left, right, TokenType::WITH, leftAttr, rightAttr));
    }

    return next;
}


PqlToken QueryExtractor::extractSuchThatClause()
{
	getNextToken(); // THAT

    PqlToken suchThatClause;
    PqlToken left;
    PqlToken right;

    PqlToken next = PqlToken(TokenType::AND, "and");
    while (next.type == TokenType::AND) 
    {
	    suchThatClause = getNextToken();
	    getNextToken(); // OPEN BRACKET
	    left = extractString(getNextToken());
	    getNextToken(); // COMMA
	    right = extractString(getNextToken());
	    getNextToken(); // CLOSE BRACKET
        pq->clauses[0].push_back(Clause(suchThatClause, left, right, TokenType::SUCH_THAT));
        next = getNextToken();
    }

    return next;
}

PqlToken QueryExtractor::extractString(PqlToken token)
{
    int size = token.value.length();
    string s = "";
    if (token.type == TokenType::STRING) {
        s = token.value.substr(1, size - 2);
    }
    else if (token.type == TokenType::WILDCARD_STRING) {
        s = token.value.substr(2, size - 4);
    }
    else {
        return token;
    }

    token.value = removeWhitespace(s);
    return token;
}

string QueryExtractor::removeWhitespace(string s) {
    string newS = "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\v') {
            continue;
        }
        newS.push_back(s[i]);
    }

    if (validOperators.find(newS[0]) != validOperators.end() || validOperators.find(newS[newS.length() - 1]) != validOperators.end()) {
        throw SyntaxError("Invalid parameters");
    }
    return newS;
}

PqlToken QueryExtractor::getNextToken()
{
    if (next == size)
    {
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens->at(next);
    next = next +1;

    if (token.type == TokenType::BOOLEAN && booleanIsSynonym == true) {
        token.type = TokenType::SYNONYM;
    }
    return token;

}
