#include "QPS/QueryExtractor.h"
#include "QPS/Structures/PqlError.h"
#include "QPS/Structures/PqlToken.h"
#include "QPS/Structures/PqlQuery.h"
#include "QPS/Types/ErrorType.h"
#include "QPS/Types/TokenType.h"
#include "TestQueryExtractorUtils.h"

#include <vector>
#include <unordered_map>
#include <memory>

#include "catch.hpp"
#include "QPS/Validators/ValidatorUtils.h"
using namespace std;

bool isSameMap(unordered_map<string, TokenType> ans, unordered_map<string, TokenType> result) {
	for (const auto & [key, value] : ans) {
		if (result.find(key) == result.end()) {
			return false;
		}
		if (result[key] != value) {
			return false;
		}
	}
	return true;
}

bool isSameClauses(vector<shared_ptr<Clause>> ans, vector<shared_ptr<Clause>> result)
{
	if (ans.size() != result.size())
	{
		return false;
	}

	for (int i = 0; i != ans.size(); i++)
	{
		if (ans[i] == result[i]) {
			continue;
		}
		else
		{
			return false;
		}
	}

	return true;
}


TEST_CASE("Test declartions")
{
	unordered_map<string, TokenType> ans = {
		{"v", TokenType::VARIABLE},
		{"a", TokenType::ASSIGN}
	};

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&basic_tokens, ptr);
	sut.extractSemantics();

	REQUIRE(isSameMap(ans, ptr->declarations));
}

TEST_CASE("Test Select clause")
{
	vector<shared_ptr<SelectObject>> ans = { make_shared<SelectObject>(SelectObject(SelectType::SYNONYM, "v")) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&basic_tokens, ptr);
	sut.extractSemantics();

	REQUIRE(ptr->selectObjects == ans);
}

TEST_CASE("Test Select only no declarations")
{
	vector<shared_ptr<SelectObject>> ans = { make_shared<SelectObject>(SelectObject(SelectType::BOOLEAN)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_select_only, ptr);
	sut.extractSemantics();

	REQUIRE(ptr->selectObjects == ans);
}

TEST_CASE("Test Calls wildcards")
{
	vector<shared_ptr<SelectObject>> ans = { make_shared<SelectObject>(SelectObject(SelectType::BOOLEAN)) };
	vector<shared_ptr<Clause>> ans2 = { make_shared<Clause>(Clause(PqlToken(TokenType::CALLS, "Calls"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::SUCH_THAT)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_calls_wildcards, ptr);
	sut.extractSemantics();

	REQUIRE(ptr->selectObjects == ans);
	REQUIRE(ptr->clauses[0] == ans2);
}


TEST_CASE("Test Select boolean")
{
	vector<shared_ptr<SelectObject>> ans = { make_shared<SelectObject>(SelectObject(SelectType::BOOLEAN)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_select_boolean, ptr);
	sut.extractSemantics();

	REQUIRE(ptr->selectObjects == ans);
}

TEST_CASE("Test Select declared boolean")
{
	vector<shared_ptr<SelectObject>> ans = { make_shared<SelectObject>(SelectObject(SelectType::SYNONYM, "BOOLEAN")) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_select_declared_boolean, ptr);
	sut.extractSemantics();

	REQUIRE(ptr->selectObjects == ans);
}

TEST_CASE("Test Select attrName")
{
	vector<shared_ptr<SelectObject>> ans = { make_shared<SelectObject>(SelectObject(SelectType::ATTRNAME, "s", PqlToken(TokenType::STMTLINE, "stmt#"))) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_select_attrname, ptr);
	sut.extractSemantics();

	REQUIRE(ptr->selectObjects == ans);
}

TEST_CASE("Test Select tuple")
{
	vector<shared_ptr<SelectObject>> ans = { 
		make_shared<SelectObject>(SelectObject(SelectType::ATTRNAME, "BOOLEAN", PqlToken(TokenType::STMTLINE, "stmt#"))),
		make_shared<SelectObject>(SelectObject(SelectType::SYNONYM, "v"))
	};

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_select_tuple, ptr);
	sut.extractSemantics();

	REQUIRE(ptr->selectObjects == ans);
}


TEST_CASE("Test Pattern clause assign")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "a"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN))};

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_pattern_assign, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Pattern clause while")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "w"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_pattern_while, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Pattern clause if")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "ifs"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_pattern_if, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Pattern clause multi")
{
	vector<shared_ptr<Clause>> ans = { 
		make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "ifs"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN)),
		make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "w"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN))
	};

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_pattern_multi, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}


TEST_CASE("Test Pattern clause with String")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "a"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::STRING, "x+y"),
								  TokenType::PATTERN)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_pattern_with_string, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Pattern clause with String and whitespace")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "a"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::STRING, "x+y"),
								  TokenType::PATTERN)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_pattern_with_string_and_whitespace, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Pattern clause with Wildcard String")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "a"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD_STRING, "x+y"),
								  TokenType::PATTERN)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_pattern_with_wildcard_string, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test With clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::NONE, ""),
								  PqlToken(TokenType::SYNONYM, "p"),
								  PqlToken(TokenType::STRING, "answer"),
								  TokenType::WITH, 								
								  PqlToken(TokenType::PROCNAME, "procName"), 								  
								  PqlToken(TokenType::NONE, "")
		))};

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_with, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Mulitple With clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::NONE, ""),
								  PqlToken(TokenType::SYNONYM, "p"),
								  PqlToken(TokenType::STRING, "answer"),
								  TokenType::WITH,
								  PqlToken(TokenType::PROCNAME, "procName"),
								  PqlToken(TokenType::NONE, "")
		)),
							 make_shared<Clause>(Clause(PqlToken(TokenType::NONE, ""),
									PqlToken(TokenType::STRING, "answer2"),
									PqlToken(TokenType::SYNONYM, "p"),
								  TokenType::WITH,
								  PqlToken(TokenType::NONE, ""),
								  PqlToken(TokenType::PROCNAME, "procName")
		)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_multi_with, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Uses clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(Clause(PqlToken(TokenType::USES, "uses"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  TokenType::SUCH_THAT))) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_uses, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Modifies clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::MODIFIES, "modifies"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  TokenType::SUCH_THAT)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_modifies, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Follows clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::FOLLOWS, "follows"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  TokenType::SUCH_THAT)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_follows, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Follows* clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::FOLLOWS_A, "follows*"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  TokenType::SUCH_THAT)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_follows_a, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Parent clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::PARENT, "parent"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  TokenType::SUCH_THAT)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_parent, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Parent* clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::PARENT_A, "parent*"),
								  PqlToken(TokenType::SYNONYM, "s"),
								  PqlToken(TokenType::STATEMENT_NUM, "1"),
								  TokenType::SUCH_THAT)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_parent_a, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Calls clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::CALLS, "calls"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::SUCH_THAT)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_calls, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Calls* clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::CALLS_A, "calls*"),
								  PqlToken(TokenType::SYNONYM, "v"),
								  PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::SUCH_THAT)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_calls_a, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Pattern then Such That clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "a"),
									PqlToken(TokenType::SYNONYM, "v"),
									PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN)),
							make_shared<Clause>(Clause(PqlToken(TokenType::USES, "uses"),
									PqlToken(TokenType::STATEMENT_NUM, "1"),
									PqlToken(TokenType::SYNONYM, "v"),
								  TokenType::SUCH_THAT)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_pattern_then_such_that, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Such That then Pattern clause")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::USES, "uses"),
									PqlToken(TokenType::STATEMENT_NUM, "1"),
									PqlToken(TokenType::SYNONYM, "v"),
									TokenType::SUCH_THAT)),
							make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "a"),
									PqlToken(TokenType::SYNONYM, "v"),
									PqlToken(TokenType::WILDCARD, "_"),
								  TokenType::PATTERN)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_such_that_then_pattern, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}


TEST_CASE("Test Mulitple Pattern and Such That clauses")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "a"),
								PqlToken(TokenType::SYNONYM, "v"),
								PqlToken(TokenType::WILDCARD, "_"),
							  TokenType::PATTERN)),
						make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "a"),
								PqlToken(TokenType::SYNONYM, "v"),
								PqlToken(TokenType::WILDCARD, "_"),
							  TokenType::PATTERN)),
						make_shared<Clause>(Clause(PqlToken(TokenType::USES, "uses"),
								PqlToken(TokenType::STATEMENT_NUM, "1"),
								PqlToken(TokenType::SYNONYM, "v"),
								TokenType::SUCH_THAT)),
						make_shared<Clause>(Clause(PqlToken(TokenType::MODIFIES, "modifies"),
								PqlToken(TokenType::STATEMENT_NUM, "1"),
								PqlToken(TokenType::SYNONYM, "v"),
								TokenType::SUCH_THAT)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_multi_pattern_then_multi_such_that, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}

TEST_CASE("Test Mulitple Pattern, With and Such That clauses")
{
	vector<shared_ptr<Clause>> ans = { make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "a"),
								PqlToken(TokenType::SYNONYM, "v"),
								PqlToken(TokenType::WILDCARD, "_"),
							  TokenType::PATTERN)),
						make_shared<Clause>(Clause(PqlToken(TokenType::SYNONYM, "a"),
								PqlToken(TokenType::SYNONYM, "v"),
								PqlToken(TokenType::WILDCARD, "_"),
							  TokenType::PATTERN)),
						make_shared<Clause>(Clause(PqlToken(TokenType::NONE, ""),
								  PqlToken(TokenType::SYNONYM, "p"),
								  PqlToken(TokenType::STRING, "one"),
								  TokenType::WITH,
								  PqlToken(TokenType::PROCNAME, "procName"),
								  PqlToken(TokenType::NONE, ""))
							),
							make_shared<Clause>(Clause(PqlToken(TokenType::NONE, ""),
									PqlToken(TokenType::STRING, "two"),
									PqlToken(TokenType::SYNONYM, "p"),
								  TokenType::WITH,
								  PqlToken(TokenType::NONE, ""),
								  PqlToken(TokenType::PROCNAME, "procName"))),
						make_shared<Clause>(Clause(PqlToken(TokenType::USES, "uses"),
								PqlToken(TokenType::STATEMENT_NUM, "1"),
								PqlToken(TokenType::SYNONYM, "v"),
								TokenType::SUCH_THAT)),
						make_shared<Clause>(Clause(PqlToken(TokenType::MODIFIES, "modifies"),
								PqlToken(TokenType::STATEMENT_NUM, "1"),
								PqlToken(TokenType::SYNONYM, "v"),
								TokenType::SUCH_THAT)) };

	shared_ptr<PqlQuery> ptr = make_shared<PqlQuery>();
	QueryExtractor sut(&valid_multi_pattern_with_such_that, ptr);
	sut.extractSemantics();

	REQUIRE(isSameClauses(ans, ptr->clauses[0]));
}