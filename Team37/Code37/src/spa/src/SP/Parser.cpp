#include <vector>

using namespace std;

#include "Parser.h"
#include "Tokenizer.h"
#include "Utilities/Keywords.h"
#include "Utilities/Utils.h"
#include "AST/SourceCode.h"
#include "InvalidSyntaxException.h"

vector<string> Parser::extractProcedures(string srcCode, vector<string> procedures) {
    if (srcCode.length() == 0) {
        return procedures;
    }

    if (srcCode.substr(0, 10) == "procedure ") {
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
            if (srcCode.empty()) {
                throw InvalidSyntaxException((char *) "Missing \"}\"");
            }
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
        return extractProcedures(Utils::trim(srcCode), procedures);
    } else {
        throw InvalidSyntaxException((char *) "Incorrect procedure syntax");
    }
}

string Parser::extractProcName(string procedure) {
    string keyword = "procedure ";

    // Defensive line, can remove if we are sure procedure string is already trimmed
    procedure = Utils::trim(procedure);

    if (procedure.substr(0, 10) != keyword) {
        throw InvalidSyntaxException((char *) "Invalid procedure declaration");
    }

    procedure = procedure.substr(10);

    string procedureName;
    bool bracketHit = false;
    // Process until first open bracket
    for (char& c: procedure) {
        if (c != '{') {
            procedureName.push_back(c);
        } else {
            bracketHit = true;
            break;
        }
    }

    if (!bracketHit) {
        throw InvalidSyntaxException((char *) "Did you forget brackets in your procedure?");
    }

    procedureName = Utils::trim(procedureName);

    if (!Utils::validateName(procedureName)) {
        throw InvalidSyntaxException((char *) "Procedure name is invalid");
    }
    return procedureName;
}

vector<string> Parser::extractStatements(string statements, vector<string> statementList) {
    // Assume that the raw procedure string is already trimmed
    if (statements.length() == 0) {
        return statementList;
    }

    char currChar;
    string statement;
    int bracketCount;

    if (statements.substr(0, 2) == "if") {
        // Process between the "if" and the "{"
        while (!statements.empty()) {
            currChar = statements[0];
            statements.erase(0, 1);
            if (currChar == ';') { // Treat as assign statement
                statementList.push_back(statement);
                return extractStatements(Utils::trim(statements), statementList);
            } else if (currChar == '{') {
                statement.push_back(currChar);
                break;
            } else {
                statement.push_back(currChar);
            }
        }

        if (statements.empty()) {
            throw InvalidSyntaxException((char *) "Cannot find '{' for if statement");
        }

        // Process the then block
        bracketCount = 1;
        while (bracketCount != 0) {
            currChar = statements[0];
            statements.erase(0, 1);
            statement.push_back(currChar);

            if (currChar == '{') {
                bracketCount++;
            } else if (currChar == '}') {
                bracketCount--;
            }
        }

        // Process until the start of the else block
        while (!statements.empty()) {
            currChar = statements[0];
            statements.erase(0, 1);
            statement.push_back(currChar);
            if (currChar == '{') {
                break;
            }
        }

        if (statements.empty()) {
            throw InvalidSyntaxException((char *) "Cannot find '{' for else block in if statement");
        }

        // Process until end of else block
        bracketCount = 1;
        while (bracketCount != 0) {
            currChar = statements[0];
            statements.erase(0, 1);
            statement.push_back(currChar);

            if (currChar == '{') {
                bracketCount++;
            } else if (currChar == '}') {
                bracketCount--;
            }
        }
        statementList.push_back(statement);
        return extractStatements(Utils::trim(statements), statementList);

    } else if (statements.substr(0, 5) == "while") {
        // Process between the "while" and the "{"
        while (!statements.empty()) {
            currChar = statements[0];
            statements.erase(0, 1);
            if (currChar == ';') { // Treat as assign statement
                statementList.push_back(statement);
                return extractStatements(Utils::trim(statements), statementList);
            } else if (currChar == '{') {
                statement.push_back(currChar);
                break;
            } else {
                statement.push_back(currChar);
            }
        }

        if (statements.empty()) {
            throw InvalidSyntaxException((char *) "Cannot find '{' for while statement");
        }

        // Process until end of while block
        bracketCount = 1;
        while (bracketCount != 0) {
            currChar = statements[0];
            statements.erase(0, 1);
            statement.push_back(currChar);

            if (currChar == '{') {
                bracketCount++;
            } else if (currChar == '}') {
                bracketCount--;
            }
        }

        statementList.push_back(statement);
        return extractStatements(Utils::trim(statements), statementList);
    }

    while (true) {
        if (statements.empty()) {
            throw InvalidSyntaxException((char *) "No semicolon found for statement");
        }
        currChar = statements[0];
        statements.erase(0, 1);
        if (currChar == ';') {
            statementList.push_back(statement);
            return extractStatements(Utils::trim(statements), statementList);
        } else {
            statement.push_back(currChar);
        }
    }

    return extractStatements(statements, statementList);
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
        if (procedure.length() == 0) {
            throw InvalidSyntaxException((char *) "Invalid syntax, missing '}'");
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
    }

    if (Utils::trim(procedure).length() != 0) {
        // Return an error, procedure not valid
        throw InvalidSyntaxException((char *) "Invalid Syntax");
    }

    return Utils::trim(statements);
}

shared_ptr<SourceCode> Parser::parseSourceCode(string srcCode, string filename) {
    shared_ptr<SourceCode> sourceCodeNode = make_shared<SourceCode>(filename);
    vector<string> procedureList;
    procedureList = Parser::extractProcedures(Utils::trim(srcCode), procedureList);
    // call parseProcedure on each procedure
    for (string rawProcedure: procedureList) {
        shared_ptr<Procedure> procedureNode = Parser::parseProcedure(rawProcedure);
        sourceCodeNode->addProcedure(procedureNode);
        procedureNode->setParent(sourceCodeNode);
    }
    sourceCodeNode->setNumOfStatements(Statement::getLineNumCount());
    return sourceCodeNode;
}

shared_ptr<Procedure> Parser::parseProcedure(string procedure) {
    string procedureName = Parser::extractProcName(procedure);
    shared_ptr<Procedure> procedureNode = make_shared<Procedure>(nullptr, procedureName);

    vector<string> statementList;
    statementList = Parser::extractStatements(removeProcedureWrapper(procedure), statementList);
    for (string statement : statementList) {
        shared_ptr<Statement> statementNode = Parser::parseStatement(statement);
        procedureNode->addStatement(statementNode);
        statementNode->setParent(procedureNode);
    }
    return procedureNode;
}

shared_ptr<Statement> Parser::parseStatement(string statement) {
    statement = Utils::trim(statement);
    shared_ptr<Statement> statementNode;

    if (isAssignStatement(statement)) {
        statementNode = Tokenizer::tokenizeAssign(statement);
    } else if (statement.substr(0, 6) == "print ") {
        statementNode = Tokenizer::tokenizePrint(statement);
    } else if (statement.substr(0, 5) == "read ") {
        statementNode = Tokenizer::tokenizeRead(statement);
    } else if (statement.substr(0, 5) == "call ") {
        statementNode = Tokenizer::tokenizeCall(statement);
    } else if (statement.substr(0, 2) == "if") {
        statementNode = Parser::parseIfElse(statement);
    } else if (statement.substr(0, 5) == "while") {
        statementNode = Parser::parseWhile(statement);
    } else { }
    return statementNode;
}

// TODO: check logic
shared_ptr<IfStatement> Parser::parseIfElse(string ifElseBlock) {
    string ifStmt;
    string ifBlock;
    string elseBlock;

    // Process until first open bracket of if-else block, extracts the if statement
    while (true) {
        char nextLetter = ifElseBlock[0];
        ifStmt.push_back(nextLetter);
        ifElseBlock.erase(0, 1);
        if (nextLetter == '{') {
            break;
        }
        if (ifElseBlock.length() == 0) {
            throw InvalidSyntaxException((char *) "Invalid Syntax, no '{' found");
        }
    }

    int numBrackets = 1;

    // Do bracket counting UNTIL end of if block
    while (numBrackets != 0) {
        if (ifElseBlock.length() == 0) {
            throw InvalidSyntaxException((char *) "Invalid syntax, missing '}'");
        }
        char nextLetter = ifElseBlock[0];
        ifElseBlock.erase(0, 1);

        if (nextLetter == '{') {
            numBrackets += 1;
        } else if (nextLetter == '}') {
            numBrackets -= 1;
        }

        if (numBrackets != 0) {
            ifBlock.push_back(nextLetter);
        }
    }

    ifElseBlock = Utils::trim(ifElseBlock);

    // throw error, missing else block
    if (ifElseBlock.length() == 0) {
        throw InvalidSyntaxException((char *) "Invalid syntax, missing else block");
    }

    if (ifElseBlock.length() != 0) {
        // if else block is present, do bracket counting as well till end of else block
        if (ifElseBlock.substr(0,4) == "else") {
            // Process until first open bracket of else block
            while (true) {
                char nextLetter = ifElseBlock[0];
                ifElseBlock.erase(0, 1);
                if (nextLetter == '{') {
                    break;
                }
                if (ifElseBlock.length() == 0) {
                    throw InvalidSyntaxException((char *) "Invalid Syntax, no '{' found");
                }
            }

            numBrackets = 1;

            // Do bracket counting UNTIL end of else block
            while (numBrackets != 0) {
                if (ifElseBlock.length() == 0) {
                    throw InvalidSyntaxException((char *) "Invalid syntax, missing '}'");
                }
                char nextLetter = ifElseBlock[0];
                ifElseBlock.erase(0, 1);

                if (nextLetter == '{') {
                    numBrackets += 1;
                } else if (nextLetter == '}') {
                    numBrackets -= 1;
                }

                if (numBrackets != 0) {
                    elseBlock.push_back(nextLetter);
                }
            }
        } else {
            // Return an error, procedure not valid
            throw InvalidSyntaxException((char *) "Invalid If Else Block");
        }
    }

    ifStmt = Utils::trim(ifStmt);
    ifBlock = Utils::trim(ifBlock);
    elseBlock = Utils::trim(elseBlock);

    size_t endOfCondition = ifStmt.find_last_of(')');
    // Throw exceptions for missing then keyword, will be caught in SP::Parser()
    if (ifStmt.substr(endOfCondition, string::npos).find(Keywords::THEN) == string::npos) {
        throw InvalidSyntaxException((char *)"Syntax error: if-else must have then keyword.");
    }

    string condExpr = Parser::extractConditionalExpr(ifStmt);
    shared_ptr<ConditionalExpression> condExprNode = Parser::parseCondExpr(condExpr);
    shared_ptr<IfStatement> ifNode = make_shared<IfStatement>(nullptr, condExprNode);
    condExprNode->setParent(ifNode);

    vector<string> ifStmtLst;
    vector<string> elseStmtLst;
    ifStmtLst = Parser::extractStatements(ifBlock, ifStmtLst);
    elseStmtLst = Parser::extractStatements(elseBlock, elseStmtLst);

    for (string s: ifStmtLst) {
        shared_ptr<Statement> statement = Parser::parseStatement(s);
        ifNode->addThenStatement(statement);
        statement->setParent(ifNode);
    }

    for (string s: elseStmtLst) {
        shared_ptr<Statement> statement = Parser::parseStatement(s);
        ifNode->addElseStatement(statement);
        statement->setParent(ifNode);
    }
    return ifNode;
}

shared_ptr<WhileStatement> Parser::parseWhile(string whileBlock) {
    size_t firstEgyptianOpen = whileBlock.find_first_of(Keywords::OPEN_EGYPTIAN);
    string whileStmtStr = whileBlock.substr(0, firstEgyptianOpen);
    string condExpr = Parser::extractConditionalExpr(whileStmtStr);

    shared_ptr<ConditionalExpression> condExprNode = Parser::parseCondExpr(condExpr);
    shared_ptr<WhileStatement> whileStatement = make_shared<WhileStatement>(nullptr, condExprNode);
    condExprNode->setParent(whileStatement);

    string stmtsBlock = Parser::removeProcedureWrapper(whileBlock);
    vector<string> stmts;
    stmts = Parser::extractStatements(stmtsBlock, stmts);
    for (string s:stmts) {
        shared_ptr<Statement> statement = Parser::parseStatement(s);
        whileStatement->addStatement(statement);
        statement->setParent(whileStatement);
    }
    return whileStatement;
}

shared_ptr<RelationalExpression> Parser::parseRelExpr(string relExprStr) {
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
    } else {
        throw InvalidSyntaxException((char *) "No valid relational operator found");
    }
    relFactor1 = Tokenizer::tokenizeRelFactor(relFactorStr1);
    relFactor2 = Tokenizer::tokenizeRelFactor(relFactorStr2);
    return make_shared<RelationalExpression>(nullptr, opr, relFactor1, relFactor2);
}

shared_ptr<ConditionalExpression> Parser::parseCondExpr(string condExprStr) {
    condExprStr = Utils::stripOuterBrackets(condExprStr);
    string andOrOperators = "&|";
    string notOperator = "!";

    char nextChar;
    string expression;
    int bracketCounter = 0;

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
            if (condExprStr.length() == 0 && bracketCounter != 0) {
                throw InvalidSyntaxException((char *) "Too many open brackets in conditional expression");
            }
            continue;
        }

        // currently not in a bracket, going to next bracket
        if (nextChar == '(') {
            bracketCounter += 1;
            expression.push_back(nextChar);
            condExprStr.erase(0, 1);
            continue;
        }

        // Cannot close bracket without open
        if (nextChar == ')') {
            throw InvalidSyntaxException((char *) "Invalid syntax");
        }

        if (andOrOperators.find(nextChar) == string::npos) {
            expression.push_back(nextChar);
            condExprStr.erase(0, 1);
            continue;
        } else { // not in bracket and hit & or | operator
            shared_ptr<ConditionalExpression> leftSide = parseCondExpr(Utils::trim(expression));
            shared_ptr<ConditionalExpression> rightSide = parseCondExpr(Utils::trim(condExprStr.substr(2)));
            shared_ptr<ConditionalExpression> conditionalExpression;
            // create conditional expression according to operator
            if (condExprStr.substr(0, 2) == "&&") {
                conditionalExpression = make_shared<AndCondition>(nullptr, leftSide, rightSide);
            } else if (condExprStr.substr(0, 2) == "||") {
                conditionalExpression = make_shared<OrCondition>(nullptr, leftSide, rightSide);
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

    // Check for not operator
    while (condExprStr.length() > 0) {
        nextChar = condExprStr[0];
        // Bracket skipping
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

        if (nextChar == '(') {
            bracketCounter += 1;
            condExprStr.erase(0, 1);
            expression.push_back(nextChar);
            continue;
        }

        // Cannot close bracket without open
        if (nextChar == ')') {
            throw InvalidSyntaxException((char *) "Invalid syntax");
        }

        if (notOperator.find(nextChar) == string::npos || condExprStr.substr(0, 2) == "!=") {
            expression.push_back(nextChar);
            condExprStr.erase(0, 1);
            continue;
        } else {
            shared_ptr<ConditionalExpression> conditionalExpression = parseCondExpr(Utils::trim(condExprStr.substr(1)));
            shared_ptr<NotCondition> notCondition = make_shared<NotCondition>(nullptr, conditionalExpression);
            conditionalExpression->setParent(notCondition);

            return notCondition;
        }
    }

    // if code reaches here, there should be no && and || operators
    // Might still have brackets from it being a relation factor

    //reset variables
    condExprStr = Utils::stripOuterBrackets(expression);
    expression.clear();

    // check if is a relational expr
    if (condExprStr.find('<') != string::npos ||
        condExprStr.find("<=") != string::npos ||
        condExprStr.find('>') != string::npos ||
        condExprStr.find('<') != string::npos ||
        condExprStr.find("!=") != string::npos ||
        condExprStr.find("==") != string::npos) {
        return parseRelExpr(condExprStr);
    }
    // else throw error
    throw InvalidSyntaxException((char *) "Invalid conditional expression");
}

bool Parser::isAssignStatement(string statement) {
    int pos = statement.find("=");
    if (pos == string::npos) {
        return false;
    }

    string varName = statement.substr(0, pos);
    if ((varName.find_first_of("{}()!<>&|") != string::npos)) {
        return false;
    }
    return true;
}
