#include <vector>
#include <algorithm>

using namespace std;

#include "Tokenizer.h"
#include "SP/Utilities/Utils.h"
#include "SP/Utilities/Keywords.h"
#include "InvalidSyntaxException.h"

shared_ptr<ReadStatement> Tokenizer::tokenizeRead(string line) {
    // "read " is 5 chars
    int charsToSkip = 5;
    string varName = Utils::trim(line.substr(charsToSkip));

    if (!Utils::validateName(varName)) {
        throw InvalidSyntaxException((char *) "Variable name is invalid");
    }

    return make_shared<ReadStatement>(nullptr, varName);
}

shared_ptr<PrintStatement> Tokenizer::tokenizePrint(string line) {
    // "print " is 6 chars
    int charsToSkip = 6;
    string varName = Utils::trim(line.substr(charsToSkip));

    if (!Utils::validateName(varName)) {
        throw InvalidSyntaxException((char *) "Variable name is invalid");
    }

    return make_shared<PrintStatement>(nullptr, varName);
}

shared_ptr<AssignStatement> Tokenizer::tokenizeAssign(string line) {
    string keyword = "=";
    // find() here is ok since we are looking for first instance
    int equalsSignIndex = line.find(keyword);

    // Pull out varName
    string varName = line.substr(0, equalsSignIndex);
    varName = Utils::trim(varName);

    if (!Utils::validateName(varName)) {
        throw InvalidSyntaxException((char *) "Variable name is invalid");
    }

    // Pull out relFactor
    string rawRelationalFactor = line.substr(equalsSignIndex + 1);
    rawRelationalFactor = Utils::trim(rawRelationalFactor);

    shared_ptr<RelationalFactor> relFactor = tokenizeRelFactor(rawRelationalFactor);
    shared_ptr<AssignStatement> assignStatement = make_shared<AssignStatement>(nullptr, varName, relFactor);
    relFactor->setParent(assignStatement);
    return assignStatement;
}

shared_ptr<CallStatement> Tokenizer::tokenizeCall(string line) {
    // "call " is 5 chars
    int charsToSkip = 5;
    string procedureName = Utils::trim(line.substr(charsToSkip));

    if (!Utils::validateName(procedureName)) {
        throw InvalidSyntaxException((char *) "Procedure name is invalid");
    }

    return make_shared<CallStatement>(nullptr, procedureName);
}

shared_ptr<RelationalFactor> Tokenizer::tokenizeRelFactor(string line) {
    string highPriorityOperators = "*/%";
    string lowPriorityOperators = "+-";

    // Variable used for iteration in while loop
    char nextChar;
    // string::insert "right" side expression to this variable
    string expression;
    // Counter in case brackets are encountered. Essentially if this value is not 0 then just keep parsing
    int bracketCounter = 0;
    // Checking if some kind of outer layer of bracket was removed
    bool bracketsDetected = false;

    // Only checks low priority operators, creates operated expression if found
    while(line.length() > 0) {
        nextChar = line.back();
        // Logic to handle "skipping" when bracket counting has started
        if (bracketCounter != 0) {
            if (nextChar == ')') {
                bracketCounter += 1;
            } else if (nextChar == '(') {
                bracketCounter -= 1;
            }
            expression.insert(0, 1, nextChar);
            line.pop_back();
            continue;
        }

        if (nextChar == ')') {
            bracketCounter += 1;
            expression.insert(0, 1, nextChar);
            line.pop_back();
            bracketsDetected = true;
            continue;
        }

        if (lowPriorityOperators.find(nextChar) == string::npos) {
            expression.insert(0, 1, nextChar);
            line.pop_back();
            continue;
        } else {
            line.pop_back();
            // Recursive calls
            shared_ptr<RelationalFactor> leftSide =
                    tokenizeRelFactor(Utils::trim(line));
            shared_ptr<RelationalFactor> rightSide =
                    tokenizeRelFactor(Utils::trim(expression));
            shared_ptr<OperatedExpression> operatedExpression;
            // Create operated expressions
            if (nextChar == '+') {
                operatedExpression = make_shared<OperatedExpression>(
                        nullptr,
                        Operator::ADD,
                        leftSide,
                        rightSide);
            } else if (nextChar == '-') {
                operatedExpression = make_shared<OperatedExpression>(
                        nullptr,
                        Operator::SUBTRACT,
                        leftSide,
                        rightSide);
            }
            leftSide->setParent(operatedExpression);
            rightSide->setParent(operatedExpression);

            return operatedExpression;
        }
    }

    // Reset variables
    line = expression;
    expression.clear();
    bracketCounter = 0;
    // Another while loop that checks high priority stuff
    while(line.length() > 0) {
        nextChar = line.back();
        // Logic to handle "skipping" when bracket counting has started
        if (bracketCounter != 0) {
            if (nextChar == ')') {
                bracketCounter += 1;
            } else if (nextChar == '(') {
                bracketCounter -= 1;
            }
            if (bracketCounter != 0) { // Don't add the final bracket
                expression.insert(0, 1, nextChar);
            }
            line.pop_back();
            continue;
        }

        if (nextChar == ')') {
            bracketCounter += 1;
            // Don't add the first bracket
            line.pop_back();
            bracketsDetected = true;
            continue;
        }

        if (highPriorityOperators.find(nextChar) == string::npos) {
            expression.insert(0, 1, nextChar);
            line.pop_back();
            continue;
        } else {
            line.pop_back();
            // Recursive calls
            shared_ptr<RelationalFactor> leftSide =
                    tokenizeRelFactor(Utils::trim(line));
            shared_ptr<RelationalFactor> rightSide =
                    tokenizeRelFactor(Utils::trim(expression));
            shared_ptr<OperatedExpression> operatedExpression;
            // Create operated expressions
            if (nextChar == '*') {
                operatedExpression = make_shared<OperatedExpression>(
                        nullptr,
                        Operator::MULTIPLY,
                        leftSide,
                        rightSide);
            } else if (nextChar == '/') {
                operatedExpression = make_shared<OperatedExpression>(
                        nullptr,
                        Operator::DIVIDE,
                        leftSide,
                        rightSide);
            } else if (nextChar == '%') {
                operatedExpression = make_shared<OperatedExpression>(
                        nullptr,
                        Operator::MODULO,
                        leftSide,
                        rightSide);
            }
            leftSide->setParent(operatedExpression);
            rightSide->setParent(operatedExpression);

            return operatedExpression;
        }
    }

    if (bracketsDetected) {
        return tokenizeRelFactor(Utils::trim(expression));
    }

    // If code reaches this point there should be no more brackets or operators
    // If still no return by this point, then the relFactor is either a variable or a const

    // Reset variables
    line = expression;
    expression.clear();

    string digits = "0123456789";

    // Check if it is a variable
    nextChar = line[0];
    // Must be variable since first letter is not a DIGIT
    if (digits.find(nextChar) == string::npos) {

        if (!Utils::validateName(line)) {
            throw InvalidSyntaxException((char *) "Variable name is invalid");
        }

        return make_shared<NameExpression>(nullptr, line);
    }

    if (!Utils::validateInteger(line)) {
        throw InvalidSyntaxException((char *) "Constant value is invalid");
    }

    // stoi converts std::string to int
    return make_shared<ConstantExpression>(nullptr, stoi(line));

}
