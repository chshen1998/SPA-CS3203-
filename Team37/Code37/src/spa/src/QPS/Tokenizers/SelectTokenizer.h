using namespace std;

#ifndef TEAM37_SELECT_TOKENIZER_H
#define TEAM37_SELECT_TOKENIZER_H

#include "GeneralTokenizer.h"
#include "TokenizerUtils.h"
#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"


class SelectTokenizer : public GeneralTokenizer {

public:
	SelectTokenizer(int& index, vector<string>& delimited, vector<PqlToken>& result) :
		GeneralTokenizer(index, delimited, result) {};

	void tokenize() override;
	TokenType checkSelectTokenType(const string&, const ClauseType&);
};

#endif