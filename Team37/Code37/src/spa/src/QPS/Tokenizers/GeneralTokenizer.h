using namespace std;

#ifndef TEAM37_GENERAL_TOKENIZER_H
#define TEAM37_GENERAL_TOKENIZER_H

#include "../Structures/PqlError.h"
#include "../Structures/PqlToken.h"
#include "../Structures/PqlQuery.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"

class GeneralTokenizer {
protected:
	int& currentIndex;
	vector<PqlToken>& tokens;
	const vector<string>& delimited_query;

public:
	GeneralTokenizer(int& index, const vector<string>& delimited, vector<PqlToken>& result) :
		currentIndex(index), delimited_query(delimited), tokens(result) {};
	virtual ~GeneralTokenizer() {}; // destructor for polymorphism
	virtual void tokenize() {};
};

#endif
