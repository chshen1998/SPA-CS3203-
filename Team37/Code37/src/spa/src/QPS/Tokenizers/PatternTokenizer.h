using namespace std;

#ifndef TEAM37_PATTERN_TOKENIZER_H
#define TEAM37_PATTERN_TOKENIZER_H

#include "GeneralTokenizer.h"
#include "TokenizerUtils.h"
#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"

/*
	* For Pattern Clause: [pattern, synonym, (, entRef, comma, expression_spec, )]
	* Notice that we need to verify the token type for indexes 1 (synonym), 3 (entRef), 5 (Expression Spec)
	*/
const int patternClauseFirstArgIndex = 1;
const int patternClauseSecondArgIndex = 3;
const int patternClauseThirdArgIndex = 5;
const int patternClauseFourthArgIndex = 7; // only applicable for if-pattern clause

class PatternTokenizer : public GeneralTokenizer {

public:
	PatternTokenizer(int& index, vector<string>& delimited, vector<PqlToken>& result) :
		GeneralTokenizer(index, delimited, result) {};

	void tokenize() override;
	TokenType checkPatternTokenType(const string&, int);
};

#endif