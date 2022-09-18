using namespace std;

#ifndef TEAM37_QUERYTOKENIZER_H
#define TEAM37_QUERYTOKENIZER_H

#include "QPS.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

// The following enum classes are helper enums when deciding the tokenType of the delimited string
enum class TokenizeState {
    FINDING_KEYWORDS,
    DECLARATION,
    SELECT,
    SUCH_THAT,
    PATTERN
};

enum class SpecificClause {
    NONE, 
    USE,
    FOLLOWS,
    MODIFIES,
    PARENT,
    CALL,
    NEXT,
    AFFECT,
    ASSIGN,
    WHILE,
    IF
};

enum class ClauseArgNumber {
    NONE,
    FIRST,
    SECOND,
    THIRD
};


extern unordered_map<TokenType, SpecificClause> tokenToClauseMap;


/*
During Tokenizing of clauses, I will have a counter that tracks the index of my string.
For Such That Clause: [such, that, relationshipKeyword, (, token, comma, token, )]
*/
extern int suchThatClauseTypeIndex;
extern int suchThatClauseFirstArgIndex;
extern int suchThatClauseSecondArgIndex;

extern int patternClauseFirstArgIndex;
extern int patternClauseSecondArgIndex;
extern int patternClauseThirdArgIndex;


class QueryTokenizer {
public:
    vector<PqlToken> tokens;
    vector<string> delimited_query;
    string query;

    QueryTokenizer(string query);

    vector<PqlToken> Tokenize();
    void resetQueryString(string query);

private:
    void Split();
    void ConvertIntoTokens();
    void TokenizeBeforeSelect(int& i);
    void TokenizeAfterSelect(int& i);
};


TokenType checkTokenType(const string& s, const TokenizeState& state, const SpecificClause& type, const ClauseArgNumber& argnum);
inline TokenType getStmtRefToken(const string& s);
inline TokenType getEntRefToken(const string& s);
inline TokenType getExpressionSpec(const string& s);

inline bool checkIfSynonym(const string& s);
inline bool checkIfInteger(const string& s);
inline bool checkIfStmtRef(const string& s);
inline bool checkIfEntRef(const string& s);
inline bool checkIfString(const string& s);
inline bool checkIfWildCardString(const string& s);
inline bool checkIfExpressionSpec(const string& s);
inline bool checkIfDesignEntity(const string& s);


#endif //TEAM37_QPS_H
