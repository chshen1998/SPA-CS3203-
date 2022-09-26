using namespace std;

#ifndef TEAM37_QUERYTOKENIZER_H
#define TEAM37_QUERYTOKENIZER_H

#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

#include "QPS.h"


/*
* Represents the current state when tokenizing our delimited query
* The state corresponds to what kind of keyword/clause we are looking at
*/
enum class TokenizeState {
    FINDING_KEYWORDS, 
    SELECT,
    SUCH_THAT,
    PATTERN,
    WITH
};

/*
* Represents the type of clause in each state
* Examples include USE for such-that clauses etc.
* Others are omitted as most clauses have shared logic
*/
enum class SpecificClause {
    NONE, 

    ATTR_REF,
    TUPLE,
    BOOLEAN,

    USE,
    MODIFIES,
    CALL,

    ASSIGN,
    WHILE,
    IF
};


/*
* A mapping of token types to a specific clause to make it easier to call checkTokentype()
*/
extern unordered_map<TokenType, SpecificClause> tokenToClauseMap;


/*
* During Tokenizing of clauses, I will have a clause counter that tracks which part of the clause I am at
* For example, upon encountering "such", the counter starts at 0, and the next keyword "that" is 1,
* and the open-bracket is index 2 of the clause and so on for a such-that clause.
* Therefore, I track this index to verify which string I need to check the token type for.
* More examples below.
*/

/*
* For Such That Clause: [such, that, clauseType, (, leftArg, comma, rightArg, )]
* Notice that we need to verify the token type for indexes 2 (clauseType), 4 (leftArg), 6 (rightArg)
*/
const int suchThatClauseTypeIndex = 2;
const int suchThatClauseFirstArgIndex = 4;
const int suchThatClauseSecondArgIndex = 6;

/*
* For Pattern Clause: [pattern, synonym, (, entRef, comma, expression_spec, )]
* Notice that we need to verify the token type for indexes 1 (synonym), 3 (entRef), 5 (Expression Spec)
*/
const int patternClauseFirstArgIndex = 1;
const int patternClauseSecondArgIndex = 3;
const int patternClauseThirdArgIndex = 5;
const int patternClauseFourthArgIndex = 7; // only applicable for if


class QueryTokenizer {
public:
    vector<PqlToken> tokens;
    vector<string> delimited_query;
    string query;

    QueryTokenizer(string);

    vector<PqlToken> Tokenize();
    void resetQueryString(string query);

private:
    void Split();
    void ConvertIntoTokens();
    void TokenizeDeclarations(int& i);
    void TokenizeSelectAndClauses(int& i);
};


//TokenType checkTokenType(const string& s, const TokenizeState& state, const SpecificClause& type, const ClauseArgNumber& argnum);

TokenType checkSuchThatTokenType(const string& s, const SpecificClause& type, const int & argnum);
TokenType checkPatternTokenType(const string& s, const int& argNum);
TokenType checkSelectTokenType(const string& s, const SpecificClause& type);


inline TokenType checkDeclarationTokenType(const string& s);
inline TokenType getExpressionSpec(const string& s);
inline TokenType getStmtRefToken(const string& s);
inline TokenType getEntRefToken(const string& s);
inline TokenType getExpressionSpec(const string& s);

inline bool checkIfSynonym(const string& s);
inline bool checkIfInteger(const string& s);
inline bool checkIfString(const string& s);
inline bool checkIfWildCardString(const string& s);
inline bool checkIfDesignEntity(const string& s);
inline bool checkIfAttrName(const string& s);

#endif //TEAM37_QPS_H
