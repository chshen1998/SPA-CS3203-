#include "QPS/QuerySemanticsExtractor.h"
#include "QPS/QPS.h"

#include <vector>
#include <unordered_map>

#include "catch.hpp"
using namespace std;

extern vector<PqlToken> basic_tokens;

vector<PqlToken> addPatternClause(vector<PqlToken> tokens);

vector<PqlToken> addUsesClause(vector<PqlToken> tokens);

vector<PqlToken> addModifiesClause(vector<PqlToken> tokens);

vector<PqlToken> addParentClause(vector<PqlToken> tokens);

vector<PqlToken> addFollowsClause(vector<PqlToken> tokens);