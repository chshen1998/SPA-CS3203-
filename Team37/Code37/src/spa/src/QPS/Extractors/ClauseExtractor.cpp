#include <QPS/Validators/ValidatorUtils.h>
#include <iso646.h>

using namespace std;

#include <string>
#include <vector>
#include <set>

#include ".././Structures/PqlError.h"
#include ".././Structures/PqlToken.h"
#include ".././Structures/PqlQuery.h"
#include ".././Types/TokenType.h"
#include "BaseExtractor.h"
#include "ClauseExtractor.h"

set<char> validOperators = {
    '+', '*', '-', '/'
};

ClauseExtractor::ClauseExtractor(shared_ptr<PqlQuery> pq_ptr, vector<PqlToken>* tokenVector, bool boolean) : BaseExtractor(pq_ptr, tokenVector, boolean) {}

void ClauseExtractor::extract(int start, int last) {
    next = start;
    end = last;

    pq->clauses.push_back(vector<Clause>{});
    
    PqlToken token = getNextToken();
    while (token.type != TokenType::END)
    {
        switch (token.type) {
        case TokenType::PATTERN:
            token = extractPatternClause();
            break;
        case TokenType::WITH:
            token = extractWithClause();
            break;
        default:
            token = extractSuchThatClause();
            break;
        }
    }
}

PqlToken ClauseExtractor::extractPatternClause()
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

PqlToken ClauseExtractor::extractWithClause()
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


PqlToken ClauseExtractor::extractSuchThatClause()
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

PqlToken ClauseExtractor::extractString(PqlToken token)
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

string ClauseExtractor::removeWhitespace(string s) {
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