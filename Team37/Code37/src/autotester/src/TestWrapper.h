#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <iostream>
#include <list>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

// include your other headers here
#include "AST/TNode.h"
#include "AbstractWrapper.h"
#include "PKB/PKB.h"
#include "QPS/QPS.h"
#include "SP/FileNotFoundException.h"
#include "SP/InvalidSyntaxException.h"
#include "SP/SP.h"

using namespace std;

class TestWrapper : public AbstractWrapper {
private:
    shared_ptr<SP> sourceProcessor;
    shared_ptr<PKB> knowledgeBase;
    shared_ptr<QPS> queryProcessor;

public:
    // default constructor
    TestWrapper();

    // destructor
    ~TestWrapper();

    // method for parsing the SIMPLE source
    virtual void parse(string filename);

    // method for evaluating a query
    virtual void evaluate(string query, list<string>& results);
};

#endif
