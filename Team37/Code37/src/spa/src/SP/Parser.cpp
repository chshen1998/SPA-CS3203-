#include <vector>

using namespace std;

#include "Parser.h"
#include "Tokenizer.h"
#include "Utilities/Keywords.h"
#include "Utilities/Utils.h"
#include "AST/SourceCode.h"
#include "InvalidSyntaxException.h"

//TODO: pass in pointer to procedures instead of pass by value
vector<string> Parser::extractProcedures(string srcCode, vector<string> procedures) {
    string keyword = Keywords::PROCEDURE;

    int startIdx = srcCode.find(keyword);
    if (startIdx != -1) {
        int len = startIdx + keyword.length();
        string remaining = srcCode.substr(len, string::npos);

        int next = remaining.find(keyword);
        if (next != -1) {
            string statements = remaining.substr(0, next);
            string nextBlock = remaining.substr(next, string:: npos);

            string p = keyword + statements;
            procedures.push_back(p);
            procedures = extractProcedures(nextBlock, procedures);
        } else {
            procedures.push_back(keyword + remaining);
        }
    }
    return procedures;
}

string Parser::extractProcName(string procedure) {
    string keyword = Keywords::PROCEDURE;

    int procId = procedure.find(keyword);
    procId = procId + keyword.length();
    string bracket = "{";
    int bracketId = procedure.find(bracket);
    string name = procedure.substr(procId, bracketId - procId);
    return Utils::trim(name);
}

string Parser::removeProcedureWrapper(string procedure) {
    string statements;

    // Process until first open bracket of procedure
    while (true) {
        char nextLetter = procedure[0];
        procedure.erase(0, 1);
        if (nextLetter == '{') {
            break;
        }
    }

    int numBrackets = 1;

    // Do bracket counting UNTIL end of procedure
    while (numBrackets != 0) {
        char nextLetter = procedure[0];
        procedure.erase(0, 1);

        if (nextLetter == '{') {
            numBrackets += 1;
        } else if (nextLetter == '}') {
            numBrackets -= 1;
        }

        if (numBrackets != 0) {
            statements.push_back(nextLetter);
        }
    }

    if (Utils::trim(procedure).length() != 0) {
        // Return an error, procedure not valid
    }

    return Utils::trim(statements);
}

vector<string> Parser::extractStatements(string procedure, vector<string> statementList) {
    // Assume that the raw procedure string is already trimmed
    if (procedure.length() == 0) {
        return statementList;
    }
    // If it is a while or if statement, do bracket matching
    // Else process until semicolon
    if (procedure.substr(0, 2) == "if") {
        string statement;

        // Process until first open bracket of if block
        while (true) {
            char nextLetter = procedure[0];
            procedure.erase(0, 1);

            statement.push_back(nextLetter);
            if (nextLetter == '{') {
                break;
            }
        }

        int numBrackets = 1;

        // Do bracket counting UNTIL end of if block
        while (numBrackets != 0) {
            char nextLetter = procedure[0];
            procedure.erase(0, 1);

            statement.push_back(nextLetter);
            if (nextLetter == '{') {
                numBrackets += 1;
            } else if (nextLetter == '}') {
                numBrackets -= 1;
            }
        }

        // Process until first open bracket of else block
        while (true) {
            char nextLetter = procedure[0];
            procedure.erase(0, 1);

            statement.push_back(nextLetter);
            if (nextLetter == '{') {
                break;
            }
        }

        numBrackets = 1;

        // Do bracket counting UNTIL end of else block
        while (numBrackets != 0) {
            char nextLetter = procedure[0];
            procedure.erase(0, 1);

            statement.push_back(nextLetter);
            if (nextLetter == '{') {
                numBrackets += 1;
            } else if (nextLetter == '}') {
                numBrackets -= 1;
            }
        }

        statementList.push_back(statement);
        procedure = Utils::trim(procedure);

    } else if (procedure.substr(0, 5) == "while") {
        string statement;

        // Process until first open bracket
        while (true) {
            char nextLetter = procedure[0];
            procedure.erase(0, 1);

            statement.push_back(nextLetter);
            if (nextLetter == '{') {
                break;
            }
        }

        int numBrackets = 1;

        // Do bracket counting
        while (numBrackets != 0) {
            char nextLetter = procedure[0];
            procedure.erase(0, 1);

            statement.push_back(nextLetter);
            if (nextLetter == '{') {
                numBrackets += 1;
            } else if (nextLetter == '}') {
                numBrackets -= 1;
            }
        }

        statementList.push_back(statement);
        procedure = Utils::trim(procedure);

    } else {
        string statement;
        bool flag = true;
        // Check character by character until semicolon reached
        while (flag) {
            char nextLetter = procedure[0];
            procedure.erase(0, 1);
            if (nextLetter == ';') {
                flag = false;
            } else {
                statement.push_back(nextLetter);
            }
        }
        statementList.push_back(statement);
        procedure = Utils::trim(procedure);
    }
    return extractStatements(procedure, statementList);
}

string Parser::extractConditionalExpr(string str) {
    str = Utils::trim(str);
    size_t condExprStart = str.find_first_of(Keywords::OPEN_BRACKET) + 1;
    size_t condExprEnd = str.find_last_of(Keywords::CLOSE_BRACKET);
    size_t condExprLength = condExprEnd - condExprStart;
    return Utils::trim(str.substr(condExprStart, condExprLength));
}

string Parser::extractStatementBlock(string block, size_t firstEgyptianOpen) {
    size_t stmtLstStart = firstEgyptianOpen + 1;
    size_t stmtLstEnd = block.find_last_of(Keywords::CLOSE_EGYPTIAN);
    size_t stmtLstLength = stmtLstEnd - stmtLstStart;
    return Utils::trim(block.substr(stmtLstStart, stmtLstLength));
}

shared_ptr<RelationalExpression> Parser::parseRelExpr(string relExprStr, shared_ptr<TNode> parent) {

}

shared_ptr<ConditionalExpression> Parser::parseCondExpr(string condExprStr, shared_ptr<TNode> parent) {
    condExprStr = Utils::trim(condExprStr);

    // if no brackets, it is a relational expression
    int openIdx = condExprStr.find(Keywords::OPEN_BRACKET);
    int closedIdx = condExprStr.find(Keywords::CLOSE_BRACKET);
    if (openIdx == -1 && closedIdx == -1) {
        return Parser::parseRelExpr(condExprStr, parent);
    }

    // if not a relational expression, do bracket counting instead
    int numBrackets;
    char nextLetter = condExprStr[0];
    condExprStr.erase(0, 1);

    // Check for NOT conditional expression
    if ((nextLetter == '!') && (condExprStr[0] != '=')) {
        string condition;
        // parse until first open bracket, remove any whitespace between ! and (
        while (true) {
            nextLetter = condExprStr[0];
            condExprStr.erase(0, 1);
            if (nextLetter == '(') {
                numBrackets = 1;
                break;
            }
        }
        while (numBrackets != 0) {
            nextLetter = condExprStr[0];
            condExprStr.erase(0, 1);
            if (nextLetter == '(') {
                numBrackets += 1;
            } else if (nextLetter == ')') {
                numBrackets -= 1;
            }
            if (numBrackets != 0) {
                condition.push_back(nextLetter);
            }
        }
        if (condExprStr.length() == 0) {
            shared_ptr<ConditionalExpression> condExpr = Parser::parseCondExpr(condition, nullptr);
            shared_ptr<NotCondition> NotCondExpr = make_shared<NotCondition>(parent, condExpr);
            condExpr->setParent(NotCondExpr);
            return NotCondExpr;
        } else {
            shared_ptr<ConditionalExpression> condExpr1 = Parser::parseCondExpr(condition, nullptr);
            shared_ptr<NotCondition> NotCondExpr = make_shared<NotCondition>(parent, condExpr1);
            condExpr1->setParent(NotCondExpr);

            condExprStr = Utils::trim(condExprStr);
            if (condExprStr.substr(0, 2) == "&&") {
                string condition2;

                while (true) {
                    nextLetter = condExprStr[0];
                    condExprStr.erase(0, 1);
                    if (nextLetter == '(') {
                        numBrackets = 1;
                        break;
                    }
                }
                while (numBrackets != 0) {
                    nextLetter = condExprStr[0];
                    condExprStr.erase(0, 1);
                    if (nextLetter == '(') {
                        numBrackets += 1;
                    } else if (nextLetter == ')') {
                        numBrackets -= 1;
                    }
                    if (numBrackets != 0) {
                        condition2.push_back(nextLetter);
                    }
                }

                shared_ptr<ConditionalExpression> condExpr2 = Parser::parseCondExpr(condition2, nullptr);
                shared_ptr<AndCondition> andExpr = make_shared<AndCondition>(parent, NotCondExpr, condExpr2);
                NotCondExpr->setParent(andExpr);
                condExpr2->setParent(andExpr);
                return andExpr;
            } else if (condExprStr.substr(0, 2) == "||") {
                string condition2;

                while (true) {
                    nextLetter = condExprStr[0];
                    condExprStr.erase(0, 1);
                    if (nextLetter == '(') {
                        numBrackets = 1;
                        break;
                    }
                }
                while (numBrackets != 0) {
                    nextLetter = condExprStr[0];
                    condExprStr.erase(0, 1);
                    if (nextLetter == '(') {
                        numBrackets += 1;
                    } else if (nextLetter == ')') {
                        numBrackets -= 1;
                    }
                    if (numBrackets != 0) {
                        condition2.push_back(nextLetter);
                    }
                }

                shared_ptr<ConditionalExpression> condExpr2 = Parser::parseCondExpr(condition2, nullptr);
                shared_ptr<OrCondition> orExpr = make_shared<OrCondition>(parent, NotCondExpr, condExpr2);
                NotCondExpr->setParent(orExpr);
                condExpr2->setParent(orExpr);
                return orExpr;
            } else {
                throw InvalidSyntaxException((char *) "Invalid condition, condition must be wrapped in a pair of parentheses!");
            }
        }
    }

    // otherwise do normal bracket counting
    string condition;
    while (true) {
        nextLetter = condExprStr[0];
        condExprStr.erase(0, 1);
        if (nextLetter == '(') {
            numBrackets = 1;
            break;
        }
    }
    while (numBrackets != 0) {
        nextLetter = condExprStr[0];
        condExprStr.erase(0, 1);
        if (nextLetter == '(') {
            numBrackets += 1;
        } else if (nextLetter == ')') {
            numBrackets -= 1;
        }
        if (numBrackets != 0) {
            condition.push_back(nextLetter);
        }
    }
    if (condExprStr.length() == 0) {
        shared_ptr<ConditionalExpression> condExpr = Parser::parseCondExpr(condition, parent);
        return condExpr;
    } else {
        shared_ptr<ConditionalExpression> condExpr1 = Parser::parseCondExpr(condition, nullptr);

        condExprStr = Utils::trim(condExprStr);
        if (condExprStr.substr(0, 2) == "&&") {
            string condition2;

            while (true) {
                nextLetter = condExprStr[0];
                condExprStr.erase(0, 1);
                if (nextLetter == '(') {
                    numBrackets = 1;
                    break;
                }
            }
            while (numBrackets != 0) {
                nextLetter = condExprStr[0];
                condExprStr.erase(0, 1);
                if (nextLetter == '(') {
                    numBrackets += 1;
                } else if (nextLetter == ')') {
                    numBrackets -= 1;
                }
                if (numBrackets != 0) {
                    condition2.push_back(nextLetter);
                }
            }

            shared_ptr<ConditionalExpression> condExpr2 = Parser::parseCondExpr(condition2, nullptr);
            shared_ptr<AndCondition> andExpr = make_shared<AndCondition>(parent, condExpr1, condExpr2);
            condExpr1->setParent(andExpr);
            condExpr2->setParent(andExpr);
            return andExpr;
        } else if (condExprStr.substr(0, 2) == "||") {
            string condition2;

            while (true) {
                nextLetter = condExprStr[0];
                condExprStr.erase(0, 1);
                if (nextLetter == '(') {
                    numBrackets = 1;
                    break;
                }
            }
            while (numBrackets != 0) {
                nextLetter = condExprStr[0];
                condExprStr.erase(0, 1);
                if (nextLetter == '(') {
                    numBrackets += 1;
                } else if (nextLetter == ')') {
                    numBrackets -= 1;
                }
                if (numBrackets != 0) {
                    condition2.push_back(nextLetter);
                }
            }

            shared_ptr<ConditionalExpression> condExpr2 = Parser::parseCondExpr(condition2, nullptr);
            shared_ptr<OrCondition> orExpr = make_shared<OrCondition>(parent, condExpr1, condExpr2);
            condExpr1->setParent(orExpr);
            condExpr2->setParent(orExpr);
            return orExpr;
        } else {
            throw InvalidSyntaxException(
                    (char *) "Invalid condition, condition must be wrapped in a pair of parentheses!");
        }
    }
}

shared_ptr<IfStatement> Parser::parseIfElse(string ifElseBlock, shared_ptr<TNode> parent) {
    size_t elseIndex = ifElseBlock.find(Keywords::ELSE);
    size_t thenIndex = ifElseBlock.find(Keywords::THEN);

    // Throw exceptions for missing else or then statements, will be caught in SP::Parser()
    if ((int) elseIndex == -1) {
        throw InvalidSyntaxException((char *)"Syntax error: if-else must have else block.");
    }
    if ((int) thenIndex == -1) {
        throw InvalidSyntaxException((char *)"Syntax error: if statement must have 'then'.");
    }
    string ifBlock = ifElseBlock.substr(0, elseIndex);
    string elseBlock = ifElseBlock.substr(elseIndex, string::npos);

    size_t firstEgyptianOpen = ifElseBlock.find_first_of(Keywords::OPEN_EGYPTIAN);
    string ifStmtStr = ifElseBlock.substr(0, firstEgyptianOpen);
    string condExpr = Parser::extractConditionalExpr(ifStmtStr);
    shared_ptr<ConditionalExpression> condExprNode = Parser::parseCondExpr(condExpr, nullptr);
    shared_ptr<IfStatement> ifNode = make_shared<IfStatement>(parent, condExprNode);
    condExprNode->setParent(ifNode);
    string ifStmtBlock = Parser::extractStatementBlock(ifBlock, firstEgyptianOpen);

    vector<string> stmts;
    stmts = Parser::extractStatements(ifStmtBlock, stmts);

    for (auto s: stmts) {
        shared_ptr<Statement> statement = Parser::parseStatement(s, ifNode);
        ifNode->addThenStatement(statement);
        statement->setParent(ifNode);
    }

    firstEgyptianOpen = elseBlock.find_first_of(Keywords::OPEN_EGYPTIAN);
    string elseStmtBlock = Parser::extractStatementBlock(elseBlock, firstEgyptianOpen);
    stmts.clear();
    stmts = Parser::extractStatements(elseStmtBlock, stmts);
    for (auto s: stmts) {
        shared_ptr<Statement> statement = Parser::parseStatement(s, ifNode);
        ifNode->addElseStatement(statement);
        statement->setParent(ifNode);
    }
    return ifNode;
}

shared_ptr<WhileStatement> Parser::parseWhile(string whileBlock, shared_ptr<TNode> parent) {
    size_t firstEgyptianOpen = whileBlock.find_first_of(Keywords::OPEN_EGYPTIAN);
    string whileStmtStr = whileBlock.substr(0, firstEgyptianOpen);
    string condExpr = Parser::extractConditionalExpr(whileStmtStr);

    shared_ptr<ConditionalExpression> condExprNode = Parser::parseCondExpr(condExpr, nullptr);
    shared_ptr<WhileStatement> whileStatement = make_shared<WhileStatement>(parent, condExprNode);
    condExprNode->setParent(whileStatement);

    string stmtsBlock = Parser::extractStatementBlock(whileBlock, firstEgyptianOpen);
    vector<string> stmts;
    stmts = Parser::extractStatements(stmtsBlock, stmts);
    for (auto s:stmts) {
        shared_ptr<Statement> statement = Parser::parseStatement(s, whileStatement);
        whileStatement->addStatement(statement);
        statement->setParent(whileStatement);
    }
    return whileStatement;
}

shared_ptr<Statement> Parser::parseStatement(string statement, shared_ptr<TNode> parentNode) {
    // TODO: split into cases for (print, read, call, if-else, while, assign)
    statement = Utils::trim(statement);
    shared_ptr<Statement> statementNode;
    if ((int)statement.find("print") == 0) {
        statementNode = Tokenizer::tokenizePrint(statement, parentNode); //TODO remove stmtNo as param in tokenizePrint
    }
    if ((int)statement.find("read") == 0) {
        statementNode = Tokenizer::tokenizeRead(statement, parentNode); //TODO remove stmtNo as param in tokenizeRead
    }
//    if ((int)statement.find("call") == 0) {
//        statementNode = Tokenizer::tokenizeCall(statement, parentNode);// TODO tokenizeCall not implemented yet
//    }
    if ((int)statement.find("if") == 0) {
        statementNode = Parser::parseIfElse(statement, parentNode);
    }
    if ((int)statement.find("while") == 0) {
        statementNode = Parser::parseWhile(statement, parentNode);
    }
    return statementNode;
}

shared_ptr<Procedure> Parser::parseProcedure(string procedure, shared_ptr<SourceCode> srcCodeNode) {
    vector<string> statementList;
    statementList = Parser::extractStatements(removeProcedureWrapper(procedure), statementList);
    string procedureName = Parser::extractProcName(procedure);
    shared_ptr<Procedure> procedureNode = make_shared<Procedure>(srcCodeNode, procedureName);
    for (auto stmt : statementList) {
        shared_ptr<Statement> statement = Parser::parseStatement(stmt, procedureNode);
        procedureNode->addStatement(statement);
        statement->setParent(procedureNode);
    }
    return procedureNode;
}

shared_ptr<SourceCode> Parser::parseSourceCode(string srcCode, string filepath) {
    shared_ptr<SourceCode> sourceCodeNode = make_shared<SourceCode>(filepath);
    vector<string> procedureList;
    procedureList = Parser::extractProcedures(srcCode, procedureList);
    // call parseProcedure on each procedure
    for (auto p: procedureList) {
        shared_ptr<Procedure> procedure = Parser::parseProcedure(p, sourceCodeNode);
        sourceCodeNode->addProcedure(procedure);
        procedure->setParent(sourceCodeNode);
    }
    return sourceCodeNode;
}

// extract statements w bracket pairing
//vector<string> SP:: extractStatements(string procedure) {
//    vector<string> statements;
//    vector<int> openIndexes;
//    vector<int> closeIndexes;
//    openIndexes = Utils::getOpenIndexes(procedure, openIndexes, 0);
//    closeIndexes = Utils::getClosedIndexes(procedure, closeIndexes, 0);
//
//    if (openIndexes.size() != closeIndexes.size()){
//        // throw error
//        if(openIndexes.size() < closeIndexes.size()) {
//            cout << "Missing {" << endl;
//        } else {
//            cout << "Missing }" << endl;
//        }
//    } else {
//        vector<vector<int> > allPairs = Utils::getSets(openIndexes, closeIndexes);
//        sort(allPairs.begin(), allPairs.end());
//        allPairs.erase(allPairs.begin()); // remove first set of brackets as it belongs to procedure
//        // group if/else/while
//    }
//}