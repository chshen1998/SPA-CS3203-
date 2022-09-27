    using namespace std;

#include <string>
#include <unordered_map>
#include <vector>

#include "../Types/TokenType.h"

#ifndef TEAM37_PQLTOKEN_H
#define TEAM37_PQLTOKEN_H

/*
 * PqlTokens are created after tokenizing an element from query string.
 * PqlToken contains the string value from the query string and the token type.
 */
struct PqlToken {
    TokenType type;
    string value;

    PqlToken(TokenType tokenType, string tokenValue) {
        type = tokenType;
        value = tokenValue;
    }

    bool operator==(const PqlToken& other) const {
        return (other.type == type) && (other.value == value);
    }

public:
    // For debugging when writing unit tests
    friend ostream& operator<< (std::ostream& os, const PqlToken& token);
       
};


#endif 