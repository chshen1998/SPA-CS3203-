#include <iso646.h>

using namespace std;

#include <string>
#include <vector>
#include <set>
#include <memory>

#include "QueryExtractor.h"
#include "./Extractors/BaseExtractor.h"
#include "./Extractors/ClauseExtractor.h"
#include "./Extractors/DeclarationExtractor.h"
#include "./Extractors/SelectExtractor.h"
#include "./Structures/PqlError.h"
#include "./Structures/PqlToken.h"
#include "./Structures/PqlQuery.h"
#include "./Types/ErrorType.h"
#include <iostream>
#include "./Types/TokenType.h"
#include "Validators/ValidatorUtils.h"

QueryExtractor::QueryExtractor(vector<PqlToken>* tokenVector, shared_ptr<PqlQuery> pq_pointer)
{
    tokens = tokenVector;
    next = 0;
    size = tokenVector->size();
    pq = pq_pointer;
    booleanIsSynonym = false;
}

void QueryExtractor::extractSemantics()
{
    extractDeclarations();

    checkBooleanSynonym();

    extractSelect();

    extractClauses();
}

void QueryExtractor::extractDeclarations()
{   
    int start = next;
    while (tokens->at(next).type != TokenType::DECLARATION_END) {
        next += 1;
    }

    shared_ptr<BaseExtractor> extractor = shared_ptr<BaseExtractor>(new DeclarationExtractor(pq, tokens));
    extractor->extract(start, next);
    next += 1;
}

void QueryExtractor::extractSelect()
{
    int start = next;
    while (next < size
        && tokens->at(next).type != TokenType::PATTERN 
        && tokens->at(next).type != TokenType::SUCH 
        && tokens->at(next).type != TokenType::WITH) {
        next += 1;
    }

    shared_ptr<BaseExtractor> extractor = shared_ptr<BaseExtractor>(new SelectExtractor(pq, tokens, booleanIsSynonym));
    extractor->extract(start, next);
}

void QueryExtractor::extractClauses()
{   
    shared_ptr<BaseExtractor> extractor = shared_ptr<BaseExtractor>(new ClauseExtractor(pq, tokens, booleanIsSynonym));
    extractor->extract(next, size);
}

void QueryExtractor::checkBooleanSynonym()
{
    if (pq->declarations.find("BOOLEAN") != pq->declarations.end()) {
        booleanIsSynonym = true;
    }
}