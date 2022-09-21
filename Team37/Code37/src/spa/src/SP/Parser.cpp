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
    if (srcCode.length() == 0) {
        return procedures;
    }

    if (srcCode.substr(0, 9) == Keywords::PROCEDURE) {
        string procedure;

        // Process until first open bracket
        while (true) {
            char nextChar = srcCode[0];
            procedure.push_back(nextChar);
            srcCode.erase(0, 1);
            if (nextChar == '{') {
                break;
            }
        }

        int bracketCounter = 1;

        // Process until close bracket
        while (bracketCounter != 0) {
            char nextChar = srcCode[0];
            procedure.push_back(nextChar);
            srcCode.erase(0, 1);
            if (nextChar == '{') {
                bracketCounter += 1;
            }
            if (nextChar == '}') {
                bracketCounter -= 1;
            }
        }
        procedures.push_back(procedure);
        return (Utils::trim(srcCode), procedures);
    } else {
        throw InvalidSyntaxException((char *) "Incorrect procedure syntax");
    }
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
        if (procedure.length() == 0) {
            throw InvalidSyntaxException((char *) "Invalid Syntax, no '{' found");
        }
    }

    int numBrackets = 1;

    // Do bracket counting UNTIL end of procedure
    while (numBrackets != 0) {
        if (numBrackets < 0) {
            throw InvalidSyntaxException((char *)"Invalid Syntax, extra '}' found");
        }
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
        if (procedure.length() == 0 && numBrackets > 0) {
            throw InvalidSyntaxException((char *) "Invalid syntax, missing '}'");
        }
    }

    if (Utils::trim(procedure).length() != 0) {
        // Return an error, procedure not valid
        throw InvalidSyntaxException((char *) "Invalid Syntax");
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
            if (procedure.length() == 0) {
                throw InvalidSyntaxException((char *) "Invalid Syntax, no '{' found");
            }
        }

        int numBrackets = 1;

        // Do bracket counting UNTIL end of if block
        while (numBrackets != 0) {
            if (numBrackets < 0) {
                throw InvalidSyntaxException((char *)"Invalid Syntax, extra '}' found");
            }
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
            if (procedure.length() == 0) {
                break;
            }
            char nextLetter = procedure[0];
            procedure.erase(0, 1);

            statement.push_back(nextLetter);
            if (nextLetter == '{') {
                break;
            }
            if (procedure.length() == 0) {
                throw InvalidSyntaxException((char *) "Invalid Syntax, no '{' found");
            }
        }

        numBrackets = 1;

        // Do bracket counting UNTIL end of else block
        while (numBrackets != 0) {
            if (procedure.length() == 0) {
                break;
            }
            if (numBrackets < 0) {
                throw InvalidSyntaxException((char *)"Invalid Syntax, extra '}' found");
            }
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
            if (procedure.length() == 0) {
                throw InvalidSyntaxException((char *) "Invalid Syntax, no '{' found");
            }
        }

        int numBrackets = 1;

        // Do bracket counting
        while (numBrackets != 0) {
            if (numBrackets < 0) {
                throw InvalidSyntaxException((char *)"Invalid Syntax, extra '}' found");
            }
            char nextLetter = procedure[0];
            procedure.erase(0, 1);

            statement.push_back(nextLetter);
            if (nextLetter == '{') {
                numBrackets += 1;
            } else if (nextLetter == '}') {
                numBrackets -= 1;
            }
            if (procedure.length() == 0 && numBrackets >0) {
                throw InvalidSyntaxException((char *) "Invalid Syntax, no '}' found");
            }
        }

        statementList.push_back(statement);
        procedure = Utils::trim(procedure);

    } else {
        string statement;
        bool flag = true;
        // Check character by character until semicolon reached
        while (flag) {
            if (procedure.length() == 0) {
                throw InvalidSyntaxException((char *) "Syntax Error, missing semicolon");
            }
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
    string condition = Utils::trim(str.substr(condExprStart, condExprLength));
    char nextChar;
    int bracketCount = 0;
    string copy = condition;
    while (copy.length() > 0) {
        nextChar = copy[0];
        if (nextChar == '(') {
            bracketCount += 1;
        } else if (nextChar == ')') {
            bracketCount -= 1;
        }
        copy.erase(0, 1);
    }
    if (bracketCount > 0) {
        throw InvalidSyntaxException((char *) "Invalid Syntax, missing '}'");
    } else if (bracketCount < 0) {
        throw InvalidSyntaxException((char *) "Invalid Syntax, missing '{'");
    } else {
        return condition;
    }
}

string Parser::extractStatementBlock(string block, size_t firstEgyptianOpen) {
    size_t stmtLstStart = firstEgyptianOpen + 1;
    size_t stmtLstEnd = block.find_last_of(Keywords::CLOSE_EGYPTIAN);
    if (stmtLstEnd == string::npos) {
        throw InvalidSyntaxException((char *) "Syntax Error, no '}' found");
    }
    size_t stmtLstLength = stmtLstEnd - stmtLstStart;
    return Utils::trim(block.substr(stmtLstStart, stmtLstLength));
}

shared_ptr<RelationalExpression> Parser::parseRelExpr(string relExprStr, shared_ptr<TNode> parent) {
    int openIdx = relExprStr.find("(");
    int closeIdx = relExprStr.find_last_of(")");
    if (openIdx == 0 && closeIdx != -1) {
        relExprStr = relExprStr.substr(openIdx + 1, closeIdx - openIdx - 1);
    }
    int greaterEqIdx = relExprStr.find(">=");
    int lessEqIdx = relExprStr.find("<=");
    int notEqIdx = relExprStr.find("!=");
    int eqIdx = relExprStr.find("==");
    int greaterIdx = relExprStr.find(">");
    int lesserIdx = relExprStr.find("<");

    RelationalOperator opr;
    shared_ptr<RelationalFactor> relFactor1;
    shared_ptr<RelationalFactor> relFactor2;
    string relFactorStr1;
    string relFactorStr2;

    if (greaterEqIdx != -1) {
        opr = RelationalOperator::GREATER_THAN_OR_EQUALS;
        relFactorStr1 = Utils::trim(relExprStr.substr(0, greaterEqIdx));
        relFactorStr2 = Utils::trim(relExprStr.substr(greaterEqIdx + 2, string::npos));
    } else if (lessEqIdx != -1) {
        opr = RelationalOperator::LESS_THAN_OR_EQUALS;
        relFactorStr1 = Utils::trim(relExprStr.substr(0, lessEqIdx));
        relFactorStr2 = Utils::trim(relExprStr.substr(lessEqIdx + 2, string::npos));
    } else if (notEqIdx != -1) {
        opr = RelationalOperator::NOT_EQUALS;
        relFactorStr1 = Utils::trim(relExprStr.substr(0, notEqIdx));
        relFactorStr2 = Utils::trim(relExprStr.substr(notEqIdx + 2, string::npos));
    } else if (eqIdx != -1) {
        opr = RelationalOperator::EQUALS;
        relFactorStr1 = Utils::trim(relExprStr.substr(0, eqIdx));
        relFactorStr2 = Utils::trim(relExprStr.substr(eqIdx + 2, string::npos));
    } else if (greaterIdx != -1) {
        opr = RelationalOperator::GREATER_THAN;
        relFactorStr1 = Utils::trim(relExprStr.substr(0, greaterIdx));
        relFactorStr2 = Utils::trim(relExprStr.substr(greaterIdx + 1, string::npos));
    } else if (lesserIdx != -1) {
        opr = RelationalOperator::LESS_THAN;
        relFactorStr1 = Utils::trim(relExprStr.substr(0, lesserIdx));
        relFactorStr2 = Utils::trim(relExprStr.substr(lesserIdx + 1, string::npos));
    }
    relFactor1 = Tokenizer::tokenizeRelFactor(relFactorStr1);
    relFactor2 = Tokenizer::tokenizeRelFactor(relFactorStr2);
    return make_shared<RelationalExpression>(parent, opr, relFactor1, relFactor2);
}

shared_ptr<ConditionalExpression> Parser::parseCondExpr(string condExprStr, shared_ptr<TNode> parent) {
    condExprStr = Utils::trim(condExprStr);
    string andOrOperators = "&&||";
    string notOperator = "!";

    char nextChar;
    string expression;
    int bracketCounter = 0;
    bool bracketsDetected = false;

    // check for andOrOperators first, creates AndCondition or OrCondition if found
    while (condExprStr.length() > 0) {
        nextChar = condExprStr[0];
        // Bracket skipping begins
        if (bracketCounter != 0) {
            if (nextChar == '(') {
                bracketCounter += 1;
            } else if (nextChar == ')') {
                bracketCounter -= 1;
            }
            expression.push_back(nextChar);
            condExprStr.erase(0, 1);
            continue;
        }

        // currently not in a bracket, going to next bracket
        if (nextChar == '(') {
            bracketCounter += 1;
            expression.push_back(nextChar);
            condExprStr.erase(0, 1);
            bracketsDetected = true;
            continue;
        }

        if (andOrOperators.find(nextChar) == string::npos) {
            expression.push_back(nextChar);
            condExprStr.erase(0, 1);
            continue;
        } else { // not in bracket and hit && or || operator
            shared_ptr<ConditionalExpression> leftSide = parseCondExpr(Utils::trim(expression), nullptr);
            shared_ptr<ConditionalExpression> rightSide = parseCondExpr(Utils::trim(condExprStr.substr(2)), nullptr);
            shared_ptr<ConditionalExpression> conditionalExpression;
            // create conditional expression according to operator
            if (condExprStr.substr(0, 2) == "&&") {
                conditionalExpression = make_shared<AndCondition>(parent, leftSide, rightSide);
            } else if (condExprStr.substr(0, 2) == "||") {
                conditionalExpression = make_shared<OrCondition>(parent, leftSide, rightSide);
            } else {
                // if & or | found but not &&, ||
                throw InvalidSyntaxException((char *) "Invalid conditional operator");
            }
            leftSide->setParent(conditionalExpression);
            rightSide->setParent(conditionalExpression);

            return conditionalExpression;
        }
    }

    // Reset variables
    condExprStr = expression;
    expression.clear();
    bracketCounter = 0;

    // check for not operator
    while (condExprStr.length() > 0) {
        nextChar = condExprStr[0];
        // skip brackets
        if (bracketCounter != 0) {
            if (nextChar == '(') {
                bracketCounter += 1;
            } else if (nextChar == ')') {
                bracketCounter -= 1;
            }
            if (bracketCounter != 0) {
                expression.push_back(nextChar);
            }
            condExprStr.erase(0, 1);
            continue;
        }

        if (nextChar == '(') {
            bracketCounter += 1;
            condExprStr.erase(0, 1); // does not add to expression this time
            bracketsDetected = true;
            continue;
        }

        if (notOperator.find(nextChar) == string::npos ||
            condExprStr.substr(0, 2) == "!=") {
            expression.push_back(nextChar);
            condExprStr.erase(0, 1);
            continue;
        } else {
            shared_ptr<ConditionalExpression> conditionalExpression = parseCondExpr(Utils::trim(condExprStr.substr(1)), nullptr);
            shared_ptr<NotCondition> notCondition = make_shared<NotCondition>(parent, conditionalExpression);
            conditionalExpression->setParent(notCondition);

            return notCondition;
        }
    }

    if (bracketsDetected) {
        return parseCondExpr(Utils::trim(expression), parent);
    }

    // if code reaches here, there should be no more brackets or &&,||operators

    //reset variables
    condExprStr = expression;
    expression.clear();

    // check if is a relational expr
    nextChar = condExprStr[0];
    if (condExprStr.find("<") != string::npos ||
        condExprStr.find("<=") != string::npos ||
        condExprStr.find(">") != string::npos ||
        condExprStr.find("<") != string::npos ||
        condExprStr.find("!=") != string::npos ||
        condExprStr.find("==") != string::npos) {
        return parseRelExpr(condExprStr, parent);
    }
    // else throw error
    throw InvalidSyntaxException((char *) "Invalid conditional expression");
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
//    string ifStmtBlock = Parser::extractStatementBlock(ifBlock, firstEgyptianOpen);
    string ifStmtBlock = Parser::removeProcedureWrapper(ifBlock);

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

//    string stmtsBlock = Parser::extractStatementBlock(whileBlock, firstEgyptianOpen);
    string stmtsBlock = Parser::removeProcedureWrapper(whileBlock);
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
    if (statement.substr(0, 5) == "print") {
        statementNode = Tokenizer::tokenizePrint(statement, parentNode);
    } else if (statement.substr(0, 4) == "read") {
        statementNode = Tokenizer::tokenizeRead(statement, parentNode);
    } else if (statement.substr(0, 4) == "call") {
//      statementNode = Tokenizer::tokenizeCall(statement, parentNode);// TODO tokenizeCall not implemented yet
    } else if (statement.substr(0, 2) == "if") {
        statementNode = Parser::parseIfElse(statement, parentNode);
    } else if (statement.substr(0, 5) == "while") {
        statementNode = Parser::parseWhile(statement, parentNode);
    } else {
        // otherwise is an assign statement
        statementNode = Tokenizer::tokenizeAssign(statement, parentNode);
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
    procedureList = Parser::extractProcedures(Utils::trim(srcCode), procedureList);
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