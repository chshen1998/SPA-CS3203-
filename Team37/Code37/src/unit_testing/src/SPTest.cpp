#include "SP/SP.h"
#include "SP/Tokenizer.h"

#include <catch.hpp>
#include <string>
#include <vector>

using namespace std;

void require(bool b);

TEST_CASE("File to String") {
    string filepath = "../Team37/Tests37/Easy.txt";
    string result = SP::fileToString(filepath);
    string expected = "procedure main {\n"
                      "    flag = 0;\n"
                      "    print flag;\n"
                      "    read flag;\n"
                      "\n"
                      "    x = 1;\n"
                      "    y = 2;\n"
                      "    print x;\n"
                      "}";
    require(result == expected);
}

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
    result = SP::extractProcedures(p, result);

    require(result == procedures);
}

TEST_CASE("Extract procedure names") {
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
    result = SP::extractProcNames(procedures);

    vector<string> expected;
    string name = "main";
    expected.push_back(name);

    require(expected == result);
}

TEST_CASE("Extract Statements") {
    string procedure = "procedure main {\n"
                       "    flag = 0;\n"
                       "    print flag;\n"
                       "    read flag;\n"
                       "\n"
                       "    x = 1;\n"
                       "    y = 2;\n"
                       "    print x;\n"
                       "}";
    vector<string> result = SP::extractStatements(procedure);
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

    require(result == readStatement);
}

TEST_CASE("Tokenize print") {
    shared_ptr<SourceCode> sourceCode = make_shared<SourceCode>("");
    shared_ptr<Procedure> procedure1 = make_shared<Procedure>(sourceCode, "p1");
    shared_ptr<PrintStatement> printStatement = make_shared<PrintStatement>(procedure1, 1, "v");

    string line = "print v";
    shared_ptr<PrintStatement> result = Tokenizer::tokenizePrint(line, 1, procedure1);

    require(result == printStatement);
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
    shared_ptr<Procedure> procedure2 = make_shared<Procedure>(sourceCode, "p1");
    shared_ptr<PrintStatement> print1 = make_shared<PrintStatement>(procedure2, 1, "flag");
    shared_ptr<ReadStatement> read1 = make_shared<ReadStatement>(procedure2, 2, "flag");
    procedure2->addStatement(print1);
    procedure2->addStatement(read1);

    vector<shared_ptr<Statement> > resultStatements = result[0]->getStatements();

    require(resultStatements[0] == print1);
    require(resultStatements[1] == read1);

}

TEST_CASE("Tokenize Procedures") {

}

TEST_CASE("Tokenize SourceCode") {

}