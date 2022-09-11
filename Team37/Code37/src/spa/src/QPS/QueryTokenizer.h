using namespace std;

#ifndef TEAM37_QUERYTOKENIZER_H
#define TEAM37_QUERYTOKENIZER_H

#include "QPS.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

enum class TokenizeState {
    FINDING_KEYWORDS,
    DECLARATION,
    SELECT,
    SUCH_THAT,
    PATTERN
};


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


TokenType checkTokenType(string s, TokenType token, TokenizeState state, bool firstArg);
inline TokenType getStmtRefToken(string s);
inline TokenType getEntRefToken(string s);
inline TokenType getExpressionSpec(string s);

inline bool checkIfSynonym(string s);
inline bool checkIfInteger(string s);
inline bool checkIfStmtRef(string s);
inline bool checkIfEntRef(string s);
inline bool checkIfString(string s);
inline bool checkIfWildCardString(string s);
inline bool checkIfExpressionSpec(string s);


inline bool checkIfDesignEntity(string s);


#endif //TEAM37_QPS_H
