#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <list>
#include <memory>
#include <vector>

// include your other headers here
#include "AbstractWrapper.h"
#include "AST/TNode.h"
#include "QPS/QPS.h"
#include "PKB/PKB.h"
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
