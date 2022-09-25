using namespace std;

#ifndef TEAM37_QUERYTOKENIZER_H
#define TEAM37_QUERYTOKENIZER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

#include "QPS.h"


// The following enum classes are helper enums when deciding the tokenType of the delimited string
enum class TokenizeState {
    FINDING_KEYWORDS,
    SELECT,
    SUCH_THAT,
    PATTERN,
    WITH
};

enum class SpecificClause {
    NONE, 
    USE,
    MODIFIES,
    CALL,
    ASSIGN,
    WHILE,
    IF
};

extern unordered_map<TokenType, SpecificClause> tokenToClauseMap;


/*
During Tokenizing of clauses, I will have a counter that tracks the index of my string.
For Such That Clause: [such, that, relationshipKeyword, (, token, comma, token, )]
For Pattern Clause: [pattern, synonym, (, entRef, comma, expression_spec, )]
*/
const int suchThatClauseTypeIndex = 2;
const int suchThatClauseFirstArgIndex = 4;
const int suchThatClauseSecondArgIndex = 6;

const int patternClauseFirstArgIndex = 1;
const int patternClauseSecondArgIndex = 3;
const int patternClauseThirdArgIndex = 5;


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


inline TokenType checkDeclarationTokenType(const string& s);
inline TokenType checkSelectTokenType(const string& s);
inline TokenType getExpressionSpec(const string& s);
inline TokenType getStmtRefToken(const string& s);
inline TokenType getEntRefToken(const string& s);
inline TokenType getExpressionSpec(const string& s);

inline bool checkIfSynonym(const string& s);
inline bool checkIfInteger(const string& s);
inline bool checkIfString(const string& s);
inline bool checkIfWildCardString(const string& s);
inline bool checkIfDesignEntity(const string& s);

#endif //TEAM37_QPS_H
