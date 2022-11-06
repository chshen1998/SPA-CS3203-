#include "AST/Statement/AssignStatement.h"
#include "AST/Statement/CallStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/WhileStatement.h"

#include "AST/Expression/ConditionalExpression/RelationalExpression.h"
#include "AST/Expression/RelationalFactor/ConstantExpression.h"

#include "AST/Operators/RelationalOperator.h"

#include "PKB/QueryServicer.h"
#include "PKB/Storage.h"

#include "SP/Tokenizer.h"

#include "catch.hpp"

using namespace std;

TEST_CASE("Parsing bracketed relational factors")
{
    string rawRelFactor = "v + x * y + z * t";
    shared_ptr<RelationalFactor> relFactor = Tokenizer::tokenizeRelFactor(rawRelFactor);

    string generatedString = relFactor->generateString();

    REQUIRE(generatedString == "(((v) + ((x) * (y))) + ((z) * (t)))");

    deque<string> parsedRelationalFactors = QueryServicer::parseRelationalFactorString(generatedString);

    vector<string> sanitizedRelationalFactors = {};
    for (const string& parsedFactor : parsedRelationalFactors) {
        string sanitizedString = QueryServicer::sanitizeString(parsedFactor);

        sanitizedRelationalFactors.push_back(sanitizedString);
    }

    // check whether relational factor strings are sanitized and parsed correctly
    auto it = find(sanitizedRelationalFactors.begin(), sanitizedRelationalFactors.end(), "x*y");
    REQUIRE(it != sanitizedRelationalFactors.end());

    auto it2 = find(sanitizedRelationalFactors.begin(), sanitizedRelationalFactors.end(), "v+x*y");
    REQUIRE(it2 != sanitizedRelationalFactors.end());

    auto it3 = find(sanitizedRelationalFactors.begin(), sanitizedRelationalFactors.end(), "z*t");
    REQUIRE(it3 != sanitizedRelationalFactors.end());

    auto it4 = find(sanitizedRelationalFactors.begin(), sanitizedRelationalFactors.end(), "v+x*y+z*t");
    REQUIRE(it4 != sanitizedRelationalFactors.end());
}

TEST_CASE("Reverse retrieve pattern match")
{
    shared_ptr<SourceCode> sc = make_shared<SourceCode>("Filename.txt");
    shared_ptr<Procedure> procedure = make_shared<Procedure>(sc, "first procedure");
    shared_ptr<Storage> storage = make_shared<Storage>();
    shared_ptr<QueryServicer> queryServicer = make_shared<QueryServicer>(storage);

    shared_ptr<RelationalFactor> rf = Tokenizer::tokenizeRelFactor("v + x * y + z * t");

    shared_ptr<AssignStatement> assignStmt = make_shared<AssignStatement>(procedure, "a", rf);
    rf->setParent(assignStmt);

    procedure->addStatement(assignStmt);
    sc->addProcedure(procedure);
    storage->storeAST(sc);

    // Positive testcases
    REQUIRE(queryServicer->reverseRetrievePatternMatch("x * y", true).size() == 1);
    REQUIRE(queryServicer->reverseRetrievePatternMatch("v + x * y", true).size() == 1);
    REQUIRE(queryServicer->reverseRetrievePatternMatch("z * t", true).size() == 1);
    REQUIRE(queryServicer->reverseRetrievePatternMatch("v", true).size() == 1);
    REQUIRE(queryServicer->reverseRetrievePatternMatch("v + x * y + z * t", true).size() == 1);
    REQUIRE(queryServicer->reverseRetrievePatternMatch("v + x * y + z * t", false).size() == 1);

    // Negative testcases
    REQUIRE(queryServicer->reverseRetrievePatternMatch("v", false).empty());
    REQUIRE(queryServicer->reverseRetrievePatternMatch("x * y", false).empty());
    REQUIRE(queryServicer->reverseRetrievePatternMatch("v + x * y", false).empty());
    REQUIRE(queryServicer->reverseRetrievePatternMatch("z * t", false).empty());
}