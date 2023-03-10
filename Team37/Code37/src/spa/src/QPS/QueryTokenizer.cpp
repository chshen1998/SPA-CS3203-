using namespace std;

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <ctype.h>

#include "./Structures/PqlError.h"
#include "./Structures/PqlQuery.h"
#include "./Structures/PqlToken.h"
#include "./Tokenizers/DeclarationTokenizer.h"
#include "./Tokenizers/PatternTokenizer.h"
#include "./Tokenizers/SelectTokenizer.h"
#include "./Tokenizers/SuchThatTokenizer.h"
#include "./Tokenizers/TokenizerUtils.h"
#include "./Tokenizers/WithTokenizer.h"
#include "./Types/ErrorType.h"
#include "./Types/TokenType.h"
#include "./Validators/ValidatorUtils.h"
#include "QueryTokenizer.h"

using namespace TokenizerUtils;

QueryTokenizer::QueryTokenizer(string queryString)
    : query(queryString)
{
}

void QueryTokenizer::resetQueryString(string queryString)
{
    query = queryString;
}

vector<PqlToken> QueryTokenizer::tokenize()
{
    if (query.empty()) {
        throw "Invalid Query Syntax :: Query Length is zero.";
    }

    split();
    convertIntoTokens();

    return tokens;
}

void QueryTokenizer::split()
{
    string currentString;
    bool selectExists = false;
    bool insideInvertedCommas = false;
    delimited_query = vector<string>();

    // Maximum length of delimited query is the number of characters in query string by Pigeonhole Principle
    delimited_query.reserve(query.size());

    for (int i = 0; i < query.size(); i++) {
        // If the character is a blank (whitespace or tab etc)
        if (isspace(query[i])) {
            if (currentString.empty()) {
                continue;
            } else if (insideInvertedCommas) {
                currentString += query[i];
            } else {
                delimited_query.push_back(currentString);
                currentString = "";
            }
        }

        // If the character is a single character symbol, for eg brackets or comma
        else if (stringToTokenMap.find(string { query[i] }) != stringToTokenMap.end()) {
            if (!currentString.empty()) {
                delimited_query.push_back(currentString);
            }
            delimited_query.push_back(string { query[i] });
            currentString = "";
        }

        else {
            if (query[i] == '"') {
                insideInvertedCommas = !insideInvertedCommas;
            }
            currentString += query[i];
            selectExists = selectExists || currentString == "Select";
        }
    }

    if (!currentString.empty()) {
        delimited_query.push_back(currentString);
    }

    if (delimited_query.empty()) {
        throw SyntaxError("Query is blank");
    }

    if (!selectExists) {
        throw SyntaxError("Select keyword does not exist");
    }

    delimited_query.shrink_to_fit(); // Shrinking the final capacity (not size) of the vector for space optimizations

    if (!checkIfDesignEntity(delimited_query[0]) && delimited_query[0] != "Select") {
        throw SyntaxError("Query must either start with a declaration or 'Select' keyword");
    }

    if (delimited_query.back() == ";") {
        throw SyntaxError("Query String should not end with a semicolon");
    }
}

void QueryTokenizer::convertIntoTokens()
{
    tokens = vector<PqlToken>();
    tokens.reserve(delimited_query.size() + 1); // Additional 1 for `declaration end`
    int index = 0; // index of delimited_query that we are looping through

    TokenizeState currentState = TokenizeState::FINDING_KEYWORDS;
    TokenType currentToken = TokenType::UNKNOWN;

    // Initialize our tokenizers
    unique_ptr<GeneralTokenizer> tokenizer = make_unique<DeclarationTokenizer>(DeclarationTokenizer(index, delimited_query, tokens));

    tokenizer->tokenize();

    while (index < delimited_query.size()) {
        if (currentState == TokenizeState::FINDING_KEYWORDS) {
            if (stringToTokenMap.find(delimited_query[index]) != stringToTokenMap.end()) {
                currentToken = stringToTokenMap[delimited_query[index]];

                if (currentToken == TokenType::SELECT) {
                    currentState = TokenizeState::SELECT;
                }
                if (currentToken == TokenType::SUCH) {
                    currentState = TokenizeState::SUCH_THAT;
                }
                if (currentToken == TokenType::PATTERN) {
                    currentState = TokenizeState::PATTERN;
                }
                if (currentToken == TokenType::WITH) {
                    currentState = TokenizeState::WITH;
                }
            } else {
                // If this token is not where it is supposed to be, it will throw an error anyway
                currentToken = TokenType::UNKNOWN;
            }

            tokens.push_back(PqlToken(currentToken, delimited_query[index]));
            index += 1;
            continue;
        }

        else if (currentState == TokenizeState::SELECT) {
            tokenizer = make_unique<SelectTokenizer>(SelectTokenizer(index, delimited_query, tokens));
        }

        else if (currentState == TokenizeState::SUCH_THAT) {
            tokenizer = make_unique<SuchThatTokenizer>(SuchThatTokenizer(index, delimited_query, tokens));
        }

        else if (currentState == TokenizeState::PATTERN) {
            tokenizer = make_unique<PatternTokenizer>(PatternTokenizer(index, delimited_query, tokens));
        }

        // WITH CLAUSE
        else {
            tokenizer = make_unique<WithTokenizer>(WithTokenizer(index, delimited_query, tokens));
        }

        tokenizer->tokenize();
        currentState = TokenizeState::FINDING_KEYWORDS;
    }
}
