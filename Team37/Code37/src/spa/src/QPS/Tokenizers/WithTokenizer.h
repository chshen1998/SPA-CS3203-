using namespace std;

#ifndef TEAM37_WITH_TOKENIZER_H
#define TEAM37_WITH_TOKENIZER_H

#include "../Structures/PqlError.h"
#include "../Structures/PqlQuery.h"
#include "../Structures/PqlToken.h"
#include "../Types/ErrorType.h"
#include "../Types/TokenType.h"
#include "GeneralTokenizer.h"
#include "TokenizerUtils.h"

class WithTokenizer : public GeneralTokenizer {

public:
    WithTokenizer(int& index, vector<string>& delimited, vector<PqlToken>& result)
        : GeneralTokenizer(index, delimited, result) {};

    void tokenize() override;

private:
    inline bool checkIfLastElement(const string&);
    TokenType checkWithTokenType(const string&);
};

#endif