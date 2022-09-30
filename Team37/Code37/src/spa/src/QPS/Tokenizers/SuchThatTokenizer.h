using namespace std;

#ifndef TEAM37_SUCHTHAT_TOKENIZER_H
#define TEAM37_SUCHTHAT_TOKENIZER_H

#include "GeneralTokenizer.h"
#include "TokenizerUtils.h"
#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"


/*
* For Such That Clause: [such, that, clauseType, (, leftArg, comma, rightArg, )]
* Notice that we need to verify the token type for indexes 2 (clauseType), 4 (leftArg), 6 (rightArg)
*/
const int suchThatClauseTypeIndex = 2;
const int suchThatClauseFirstArgIndex = 4;
const int suchThatClauseSecondArgIndex = 6;

class SuchThatTokenizer : public GeneralTokenizer {

public:
	SuchThatTokenizer(int& index, vector<string>& delimited, vector<PqlToken>& result) :
		GeneralTokenizer(index, delimited, result) {};

	void tokenize() override;
	TokenType checkSuchThatTokenType(const string&, const ClauseType&, const int&);
};

#endif