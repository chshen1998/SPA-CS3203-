#include "SP/Tokenizer.h"
#include "SP/Parser.h"

#include <catch.hpp>
#include <string>
#include <vector>

using namespace std;

void require(bool b);

TEST_CASE("Extract Procedures") {
    vector<string> procedures;
    string p = "procedure main {\n"
                      "    flag = 0;\n"
                      "    print flag;\n"
                      "    read flag;\n"
                      "\n"
                      "    x = 1;\n"
                      "    y = 2;\n"
                      "    print x;\n"
                      "}";
    procedures.push_back(p);

    vector<string> result;
    result = Parser::extractProcedures(p, result);

    require(result == procedures);
}

TEST_CASE("Extract procedure names") {
    string procedure = "procedure main {\n"
                       "    flag = 0;\n"
                       "    print flag;\n"
                       "    read flag;\n"
                       "\n"
                       "    x = 1;\n"
                       "    y = 2;\n"
                       "    print x;\n"
                       "}";

    string result = Parser::extractProcName(procedure);

    string expectedName = "main";
    require(expectedName == result);
}

TEST_CASE("Remove procedure wrapper - Good input") {
    string procedure = "procedure main {\n"
                       "    flag = 0;\n"
                       "    print flag;\n"
                       "    read flag;\n"
                       "\n"
                       "    x = 1;\n"
                       "    y = 2;\n"
                       "    print x;\n"
                       "}";

    string result = Parser::removeProcedureWrapper(procedure);
    string expected = "flag = 0;\n"
                      "    print flag;\n"
                      "    read flag;\n"
                      "\n"
                      "    x = 1;\n"
                      "    y = 2;\n"
                      "    print x;";

    require(result == expected);
}


TEST_CASE("Extract Statements - Good input") {
    string procedure = "flag = 0;\n"
                       "    print flag;\n"
                       "    read flag;\n"
                       "\n"
                       "    x = 1;\n"
                       "    y = 2;\n"
                       "print x;";

    vector<string> statementList;

    vector<string> result = Parser::extractStatements(procedure, statementList);
    vector<string> expected;
    string stmt1 = "flag = 0";
    string stmt2 = "print flag";
    string stmt3 = "read flag";
    string stmt4 = "x = 1";
    string stmt5 = "y = 2";
    string stmt6 = "print x";

    expected.push_back(stmt1);
    expected.push_back(stmt2);
    expected.push_back(stmt3);
    expected.push_back(stmt4);
    expected.push_back(stmt5);
    expected.push_back(stmt6);

    require(result == expected);
}

TEST_CASE("Tokenize read") {
    shared_ptr<SourceCode> sourceCode = make_shared<SourceCode>("");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sourceCode, "p1");
    shared_ptr<ReadStatement> readStatement = make_shared<ReadStatement>(procedure1, 1, "v");

    string line = "read v";
    shared_ptr<ReadStatement> result = Tokenizer::tokenizeRead(line, 1, procedure1);

    require(result->getVariableName() == readStatement->getVariableName());
    require(result->getLineNum() == readStatement->getLineNum());
    require(result->getParent() == readStatement->getParent());
}

TEST_CASE("Tokenize print") {
    shared_ptr<SourceCode> sourceCode = make_shared<SourceCode>("");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sourceCode, "p1");
    shared_ptr<PrintStatement> printStatement = make_shared<PrintStatement>(procedure1, 1, "v");

    string line = "print v";
    shared_ptr<PrintStatement> result = Tokenizer::tokenizePrint(line, 1, procedure1);

    require(result->getVariableName() == printStatement->getVariableName());
    require(result->getLineNum() == printStatement->getLineNum());
    require(result->getParent() == printStatement->getParent());
}

TEST_CASE("Tokenize Statements") {
    shared_ptr<SourceCode> sourceCode = make_shared<SourceCode>("");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sourceCode, "p1");

    vector<shared_ptr<Procedure> > procedures;
    procedures.push_back(procedure1);

    vector<vector<string> > statements;
    vector<string> stmts1;

    string s1 = "print flag";
    string s2 = "read flag";

    stmts1.push_back(s1);
    stmts1.push_back(s2);

    statements.push_back(stmts1);

    vector<shared_ptr<Procedure> > result = Tokenizer::tokenizeStatements(procedures, statements);

    vector<shared_ptr<Procedure> > expected;
    shared_ptr<PrintStatement> print1 = make_shared<PrintStatement>(procedure1, 1, "flag");
    shared_ptr<ReadStatement> read1 = make_shared<ReadStatement>(procedure1, 2, "flag");

    vector<shared_ptr<Statement> > resultStatements = result[0]->getStatements();

    require(resultStatements[0]->getLineNum() == print1->getLineNum());
    require(resultStatements[0]->getParent() == print1->getParent());

    require(resultStatements[1]->getLineNum() == read1->getLineNum());
    require(resultStatements[1]->getParent() == read1->getParent());

}

TEST_CASE("Tokenize SourceCode") {
    shared_ptr<SourceCode> srcCode = make_shared<SourceCode>("");

    vector<string> names;
    string name = "ABC";
    names.push_back(name);
    shared_ptr<Procedure> p1 = make_shared<Procedure>(srcCode, name);

    vector<vector<string> > statements;
    string s1 = "print flag";
    string s2 = "read flag";
    vector<string> stmtlst;
    stmtlst.push_back(s1);
    stmtlst.push_back(s2);
    statements.push_back(stmtlst);

    shared_ptr<PrintStatement> print1 = make_shared<PrintStatement>(p1, 1, "flag");
    shared_ptr<ReadStatement> read1 = make_shared<ReadStatement>(p1, 2, "flag");

    p1->addStatement(print1);
    p1->addStatement(read1);
    srcCode->addProcedure(p1);

    shared_ptr<SourceCode> srcCode2 = make_shared<SourceCode>("");
    srcCode2 = Tokenizer::tokenize(srcCode, names, statements);

    vector<shared_ptr<Procedure> > resultProcLst = srcCode2->getProcedures();
    vector<shared_ptr<Statement> > resultStmtLst = resultProcLst[0]->getStatements();
    string resultProcName = resultProcLst[0]->getProcedureName();

    require(resultStmtLst[0]->getLineNum() == print1->getLineNum());
    require(resultStmtLst[0]->getParent() == print1->getParent());

    require(resultStmtLst[1]->getLineNum() == read1->getLineNum());
    require(resultStmtLst[1]->getParent() == read1->getParent());

}
