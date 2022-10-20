#include <iso646.h>

using namespace std;

#include <string>
#include <vector>
#include <set>

#include "QueryExtractor.h"
#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"
#include "Validators/ValidatorUtils.h"

QueryExtractor::QueryExtractor(vector<PqlToken> tokenVector)
{
    tokens = tokenVector;
    size = tokens.size();
    next = 0;
    booleanIsSynonym = false;
    pq = PqlQuery();
 
}

PqlQuery QueryExtractor::extractSemantics()
{
    extractDeclarations();

    if (pq.declarations.find("BOOLEAN") != pq.declarations.end()) {
        booleanIsSynonym = true;
    }

    PqlToken curr = extractSelect();

    extractClauses(curr);

    return pq;
}

void QueryExtractor::extractDeclarations()
{
    PqlToken declaration = getNextToken();
    while (declaration.type != TokenType::DECLARATION_END) {
        PqlToken synonym = getNextToken();
        PqlToken symbol = getNextToken();
        pq.declarations[synonym.value] = declaration.type;

        if (symbol.type == TokenType::SEMICOLON) {
            declaration = getNextToken();
        }
    }
}

PqlToken QueryExtractor::extractSelect()
{
    getNextToken(); // Select token

    PqlToken token = getNextToken();
    if (token.type == TokenType::OPEN_ARROW) {
        PqlToken comma = PqlToken(TokenType::COMMA, ",");

        // While loop ends when closed arrow is reached
        while (comma.type == TokenType::COMMA) {
            token = getNextToken(); // Get token after open arrow/comma
            comma = extractSelectObject(token); // Returns either comma or closed arrow
        }
        return getNextToken(); // Get token after closed arrow
    }
    else {
        token = extractSelectObject(token);
        return token; 
    }
}

PqlToken QueryExtractor::extractSelectObject(PqlToken curr) {
    PqlToken nextToken = getNextToken();
    if (nextToken.type == TokenType::DOT) {
        nextToken = getNextToken();
        pq.selectObjects.push_back(SelectObject(SelectType::ATTRNAME, curr.value, nextToken));
        nextToken = getNextToken();
    }
    else if (curr.type == TokenType::BOOLEAN) {
        pq.selectObjects.push_back(SelectObject(SelectType::BOOLEAN));
    }
    else {
        pq.selectObjects.push_back(SelectObject(SelectType::SYNONYM, curr.value));
    }

    return nextToken;
}

void QueryExtractor::extractClauses(PqlToken nextToken)
{
    pq.clauses.push_back(vector<Clause>{});
    if (pq.clauses.size() == 0) {
        throw SyntaxError("");
    }

    while (nextToken.type != TokenType::END)
    {
	    if (nextToken.type == TokenType::PATTERN)
	    {
            nextToken = extractPatternClause();
	    }
        else if (nextToken.type == TokenType::WITH)
        {
            nextToken = extractWithClause();
        }
        else
        {
            nextToken = extractSuchThatClause();
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
        pq.clauses[0].push_back(Clause(pattern, left, right, TokenType::PATTERN));

        if (pq.declarations[pattern.value] == TokenType::IF) {
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

        pq.clauses[0].push_back(Clause(PqlToken(TokenType::NONE, ""), left, right, TokenType::WITH, leftAttr, rightAttr));
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
        pq.clauses[0].push_back(Clause(suchThatClause, left, right, TokenType::SUCH_THAT));
        next = getNextToken();
    }

    return next;
}

PqlToken QueryExtractor::extractString(PqlToken token)
{
    string newS = "";
	if (token.type == TokenType::STRING)
	{
        string s = token.value;
        //string newS = "";
        for (int i=1; i<s.length()-1; i++)
        {
	        if (!(s[i] == ' '))
	        {
                newS.push_back(s[i]);
	        }
        }
        //string newS = s.substr(1, s.size()-2);
        //return PqlToken(TokenType::STRING, newS);
	}
    else if (token.type == TokenType::WILDCARD_STRING)
    {
        string s = token.value;
        //string newS = "";
        for (int i = 2; i < s.length() - 2; i++)
        {
            if (!(s[i] == ' '))
            {
                newS.push_back(s[i]);
            }
        }
       //string newS = s.substr(2, s.size() - 4);
        //return PqlToken(TokenType::WILDCARD_STRING, newS);
    }
    else
    {
        return token;
    }

    if (newS.length() > 0 && newS[0] == '+')
    {
        throw SemanticError("Invalid parameter: " + token.value);
    }
    return PqlToken(token.type, newS);
}

PqlToken QueryExtractor::getNextToken()
{
    if (next == size)
    {
        return PqlToken(TokenType::END, "");
    }
    PqlToken token = tokens[next];
    next = next +1;

    if (token.type == TokenType::BOOLEAN && booleanIsSynonym == true) {
        token.type = TokenType::SYNONYM;
    }
    return token;

}
