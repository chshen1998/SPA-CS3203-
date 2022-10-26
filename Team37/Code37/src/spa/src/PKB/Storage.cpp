#include "Storage.h"

// Constructor
Storage::Storage() {}

/*
Store the AST
@param: AST - Shared Pointer to AST
*/
void Storage::storeAST(shared_ptr<SourceCode> AST) {
    this->AST = AST;

    // initialising AST visitors
    shared_ptr<ExtractGeneralASTVisitor> generalAstVisitor = make_shared<ExtractGeneralASTVisitor>(shared_from_this());
    shared_ptr<ExtractParentsASTVisitor> parentsAstVisitor = make_shared<ExtractParentsASTVisitor>(shared_from_this());
    shared_ptr<ExtractFollowsASTVisitor> followsAstVisitor = make_shared<ExtractFollowsASTVisitor>(shared_from_this());
    shared_ptr<ExtractModifiesASTVisitor> modifiesAstVisitor = make_shared<ExtractModifiesASTVisitor>(
            shared_from_this());
    shared_ptr<ExtractUsesASTVisitor> usesAstVisitor = make_shared<ExtractUsesASTVisitor>(
            shared_from_this());
    shared_ptr<ExtractCallsASTVisitor> callsAstVisitor = make_shared<ExtractCallsASTVisitor>(
            shared_from_this());

    // we start by traversing the AST using a Extract AST Visitor
    AST->accept(generalAstVisitor);

    // traverse the AST using a parents AST Visitor
    AST->accept(parentsAstVisitor);
    this->buildStar(PARENT);


    // traverse the AST using a follows AST Visitor
    AST->accept(followsAstVisitor);
    this->buildStar(FOLLOWS);

    // traverse the AST using a modifies AST Visitor
    AST->accept(modifiesAstVisitor);
    this->storeCallStmtProcedure(MODIFIESPV, MODIFIESSV);


    // traverse the AST using a uses AST Visitor
    AST->accept(usesAstVisitor);
    this->storeCallStmtProcedure(USESPV, USESSV);

    // traverse the AST using a calls AST Visitor

    AST->accept(callsAstVisitor);
    this->buildStar(CALLS);
}

/*
Retrieve Stored AST
@return AST SourceCode node if AST added, nullptr otherwise
*/
shared_ptr<SourceCode> Storage::retrieveAST() {
    return this->AST;
}

// Variable
/*
Store a variable in the variable set
@param varNode NameExpression Node to store
*/
void Storage::storeVar(NameExpression varNode) {
    (this->variables).insert(varNode);
}

/* 
Retrieve all stored variables
@returns Set of Variables
*/
set<NameExpression> Storage::getAllVar() {
    return this->variables;
}


// Constant
/*
Store a constant
@param constNode a ConstantExpression Node
*/
void Storage::storeConst(ConstantExpression constNode) {
    (this->constants).insert(constNode);
}

/*
Retrieve all stored constants
@return Set of constants stored
*/
set<ConstantExpression> Storage::getAllConst() {
    return this->constants;
}

// Procedure
/*
Store a procedure
@param constNode a Procedure Node
*/
void Storage::storeProc(Procedure procNode) {
    (this->procedures).insert(procNode);
}

/*
Retrieve all stored procedure
@return Set of procedure stored
*/
set<Procedure> Storage::getAllProc() {
    return this->procedures;
}

// Statement
/*
Store a shared pointer to a statement
@param stmtNode Shared pointers to a Statement Node
*/
void Storage::storeStmt(shared_ptr<Statement> stmtNode) {
    (this->statements).insert(make_pair(stmtNode->getLineNum(), stmtNode));
}

/*
Retrieve all stored statements
@return Set of shared pointers of statements stored
*/
set<shared_ptr<Statement>> Storage::getAllStmt() {
    set<shared_ptr<Statement>> output = {};

    for (auto kv : statements) {
        output.insert(kv.second);
    }

    return output;
}

// CFG
/*
 * Stores CFGMap
 */
void Storage::storeCFGMap(shared_ptr<map<int, shared_ptr<CFGNode>>> map) {
    this->CFGMap = map;
}

/**
 * after traversing the AST we have to store the Uses(c,v) relations that was not handled in the queue
 */
void Storage::storeCallStmtProcedure(ProcVarRelationType relationProcType, StmtVarRelationType relationStmtType) {
    for (const auto &tuple: this->callStmtProcedureQueue) {
        int lineNum = get<0>(tuple);
        string parentProcedureName = get<1>(tuple);
        string calledProcedureName = get<2>(tuple);

        vector<string> storedVariablesInProcedure = this->forwardRetrieveRelation(calledProcedureName,
                                                                                  relationProcType);
        for (const auto &variable: storedVariablesInProcedure) {

            // store Relation(c,v)
            this->storeRelation(lineNum, variable, relationStmtType);
            // store Relation(p,v)
            this->storeRelation(parentProcedureName, variable, relationProcType);
        }
    }

// empty the queue after storing the relations
    this->callStmtProcedureQueue = {};
}

/*
Store Relation of a Statement-Statement Relationship (Non-star). For Relation(stmt1, stmt2)
@param stmt1 First statement
@param stmt2 Second statement
@param value Value of relation
@param type Type of relation
*/
void Storage::storeRelation(int stmt1, int stmt2, StmtStmtRelationType type) {
    switch (type) {
        case (FOLLOWS):
            Follows.store(stmt1, stmt2);
            break;
        case (PARENT):
            Parent.store(stmt1, stmt2);
            break;
        default:
            throw invalid_argument("Not a Statement-Statement Realtion");
    }
}

/*
Retrieve Statement-Statement Relation Stored. For Relation(stmt1, stmt2)
@param stmt1
@param stmt2
@param type Type of relation
@returns Value of relation stored
*/
bool Storage::retrieveRelation(int stmt1, int stmt2, StmtStmtRelationType type) {
    switch (type) {
        case (FOLLOWS):
            return Follows.retrieve(stmt1, stmt2);
        case (FOLLOWSS):
            return FollowsS.retrieve(stmt1, stmt2);
        case (PARENT):
            return Parent.retrieve(stmt1, stmt2);
        case (PARENTS):
            return ParentS.retrieve(stmt1, stmt2);
        case (NEXT): {
            vector<int> lstLineNumNexts = this->forwardComputeRelation(stmt1, NEXT);

            // search for stmt2 in all lineNum that fulfil Next(stmt1,_)
            return find(lstLineNumNexts.begin(), lstLineNumNexts.end(), stmt2) != lstLineNumNexts.end();
        }
        case (NEXTS): {
            vector<int> lstLineNumNexts = this->forwardComputeRelation(stmt1, NEXTS);

            // search for stmt2 in all lineNum that fulfil Nexts(stmt1,_)
            return find(lstLineNumNexts.begin(), lstLineNumNexts.end(), stmt2) != lstLineNumNexts.end();
        }
        case (AFFECTS): {
            // Check if both are affects
            shared_ptr<AssignStatement> stmtNode1 = dynamic_pointer_cast<AssignStatement>(statements[stmt1]);
            shared_ptr<AssignStatement> stmtNode2 = dynamic_pointer_cast<AssignStatement>(statements[stmt2]);

            if (stmtNode1 == nullptr || stmtNode2 == nullptr || stmt1 == stmt2) {
                return false;
            }

            // Check CFG path + if variable 
            shared_ptr<CFGNode> cfgNode1 = this->CFGMap->at(stmt1);
            shared_ptr<CFGNode> cfgNode2 = this->CFGMap->at(stmt2);

            shared_ptr<set<pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>>> visited = make_shared<set<pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>>>();
            
            // Check if statement 1 modifies a variable used in statement 2
            string var = stmtNode1->getVarName();

            if (!retrieveRelation(stmt2, var, USESSV)) {
                return false;
            }

            return retrieveAffectsHelper(cfgNode1, nullptr, cfgNode2, var, visited);
        }
        default:
            throw invalid_argument("Not a Statement-Statement Realtion");
    }
}

/**
Helper function for retrieve affects
@param currNode Current node to check
@param parentNode Parent node of the current node
@param targetNode Target node to ccheck for
@param var Variable to check for
@param visited Visited set of (current, parent) to prevent infinite loops
@returns Value of relation stored
*/
bool Storage::retrieveAffectsHelper(shared_ptr<CFGNode> currNode, shared_ptr<CFGNode> parentNode, shared_ptr<CFGNode> targetNode, string var, shared_ptr<set<pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>>> visited) {
    // Check if path visited before
    if (visited->find(pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>(currNode, parentNode)) == visited->end()) {
        return false;
    }
    visited->insert(pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>(currNode, parentNode));

    // If reached target nodes return true
    if (currNode == targetNode) {
        return true;
    }

    // Check if intermediate node modifies variable
    shared_ptr<Statement> tnode = dynamic_pointer_cast<Statement>(currNode->getTNode());
    if (tnode == nullptr) {
        return false;
    }

    int lineNo = tnode->getLineNum();
    if (retrieveRelation(lineNo, var, MODIFIESSV)) {
        return false;
    }

    bool result = false;
    // Recurse to child nodes
    for (const auto& childNode : currNode->getChildren()) {
        result = result || retrieveAffectsHelper(childNode, currNode, targetNode, var, visited);
    }
    return result;
}

/*
Retrieve Reverse Relation Stored. For Relation(stmt1, stmt2)
@param stmt2
@param type Type of relation
@returns All stmt1 such that Relation(stmt1, stmt2) is True
*/
vector<int> Storage::reverseRetrieveRelation(int stmt2, StmtStmtRelationType type) {
    switch (type) {
        case (FOLLOWS):
            return Follows.reverseRetrieve(stmt2);
            break;
        case (FOLLOWSS):
            return FollowsS.reverseRetrieve(stmt2);
            break;
        case (PARENT):
            return Parent.reverseRetrieve(stmt2);
            break;
        case (PARENTS):
            return ParentS.reverseRetrieve(stmt2);
            break;
        default:
            throw invalid_argument("Not a Statement-Statement Realtion");
    }
}

/*
Retrieve Forward Relation Stored. For Relation(stmt1, stmt2)
@param stmt1
@param type Type of relation
@returns All stmt2 such that Relation(stmt1, stmt2) is True
*/
vector<int> Storage::forwardRetrieveRelation(int stmt1, StmtStmtRelationType type) {
    switch (type) {
        case (FOLLOWS):
            return Follows.forwardRetrieve(stmt1);
            break;
        case (FOLLOWSS):
            return FollowsS.forwardRetrieve(stmt1);
            break;
        case (PARENT):
            return Parent.forwardRetrieve(stmt1);
            break;
        case (PARENTS):
            return ParentS.forwardRetrieve(stmt1);
            break;
        default:
            throw invalid_argument("Not a Statement-Statement Realtion");
    }
}


/**
Only called after filling up Statement-Statement, fills in the Star
@param type Type of relation

*/
void Storage::buildStar(StmtStmtRelationType type) {
    switch (type) {
        case (FOLLOWS):
            FollowsS = Follows.buildStar();
            break;
        case (PARENT):
            ParentS = Parent.buildStar();
            break;
        default:
            throw invalid_argument("Not a Statement-Statement Realtion");
    }
}

/*
Store Relation of a Statement-Variable Relationship. For Relation(stmt, var)
@param stmt Statement Number
@param var Variable Name
@param type Type of relation
*/
void Storage::storeRelation(int stmt, string var, StmtVarRelationType type) {
    switch (type) {
        case (USESSV):
            UsesSV.store(stmt, var);
            break;
        case (MODIFIESSV):
            ModifiesSV.store(stmt, var);
            break;
        case (USESSVPREDICATE):
            UsesSVPredicate.store(stmt, var);
            break;
        default:
            throw invalid_argument("Not a Statement-Variable Relation");
    }
}

/*
Retrieve Statement-Statement Relation Stored. For Relation(stmt, var)
@param stmt
@param var
@param type Type of relation
@returns Value of relation stored
*/
bool Storage::retrieveRelation(int stmt, string var, StmtVarRelationType type) {
    switch (type) {
        case (USESSV):
            return UsesSV.retrieve(stmt, var);
            break;
        case (MODIFIESSV):
            return ModifiesSV.retrieve(stmt, var);
            break;
        case (USESSVPREDICATE):
            return UsesSVPredicate.retrieve(stmt, var);
            break;
        default:
            throw invalid_argument("Not a Statement-Variable Realtion");
    }
}

/*
Retrieve Forward Relation Stored.For Relation(stmt, var)
@param stmt
@param type Type of relation
@returns All var such that Relation(stmt, var) is True
*/
vector<string> Storage::forwardRetrieveRelation(int stmt, StmtVarRelationType type) {
    switch (type) {
        case (USESSV):
            return UsesSV.forwardRetrieve(stmt);
            break;
        case (MODIFIESSV):
            return ModifiesSV.forwardRetrieve(stmt);
            break;
        case (USESSVPREDICATE):
            return UsesSVPredicate.forwardRetrieve(stmt);
            break;
        default:
            throw invalid_argument("Not a Statement-Variable Relation");
    }
}

/*
Retrieve Reverse Relation Stored.For Relation(stmt, var)
@param var
@param type Type of relation
@returns All stmt such that Relation(stmt, var) is True
*/
vector<int> Storage::reverseRetrieveRelation(string var, StmtVarRelationType type) {
    switch (type) {
        case (USESSV):
            return UsesSV.reverseRetrieve(var);
            break;
        case (MODIFIESSV):
            return ModifiesSV.reverseRetrieve(var);
            break;
        case (USESSVPREDICATE):
            return UsesSVPredicate.reverseRetrieve(var);
            break;
        default:
            throw invalid_argument("Not a Statement-Variable Relation");
    }
}

/*
Store Relation of a Procedure-Variable Relationship. For Relation(proc, var)
@param proc Procedure Name
@param var Variable Name
@param type Type of relation
*/
void Storage::storeRelation(string proc, string var, ProcVarRelationType type) {
    switch (type) {
        case (USESPV):
            UsesPV.store(proc, var);
            break;
        case (MODIFIESPV):
            ModifiesPV.store(proc, var);
            break;
        default:
            throw invalid_argument("Not a Procedure-Variable Relation");
    }
}

/*
Retrieve Procedure-Variable Relationship. For Relation(proc, var)
@param proc Procedure Name
@param var Variable Name
@param type Type of relation
@returns Value of relation stored
*/
bool Storage::retrieveRelation(string proc, string var, ProcVarRelationType type) {
    switch (type) {
        case (USESPV):
            return UsesPV.retrieve(proc, var);
            break;
        case (MODIFIESPV):
            return ModifiesPV.retrieve(proc, var);
            break;
        default:
            throw invalid_argument("Not a Procedure-Variable Realtion");
    }
}

/*
Retrieve Forward Relation Stored. For Relation(proc, var)
@param proc Procedure Name
@param type Type of relation
@returns All var such that Relation(stmt, var) is True
*/
vector<string> Storage::forwardRetrieveRelation(string proc, ProcVarRelationType type) {
    switch (type) {
        case (USESSV):
            return UsesPV.forwardRetrieve(proc);
            break;
        case (MODIFIESSV):
            return ModifiesPV.forwardRetrieve(proc);
            break;
        default:
            throw invalid_argument("Not a Procedure-Variable Relation");
    }
}

/*
Retrieve Reverse Relation Stored. For Relation(proc, var)
@param var Variable Name
@param type Type of relation
@returns All stmt such that Relation(stmt, var) is True
*/
vector<string> Storage::reverseRetrieveRelation(string var, ProcVarRelationType type) {
    switch (type) {
        case (USESSV):
            return UsesPV.reverseRetrieve(var);
            break;
        case (MODIFIESSV):
            return ModifiesPV.reverseRetrieve(var);
            break;
        default:
            throw invalid_argument("Not a Procedure-Variable Relation");
    }
}

/*
Store Relation of a Procedure-Procedure Relationship. For Relation(proc1, proc2)
@param proc Procedure Name 1
@param proc Procedure Name 2
@param type Type of relation
*/
void Storage::storeRelation(string proc1, string proc2, ProcProcRelationType type) {
    switch (type) {
        case (CALLS):
            Calls.store(proc1, proc2);
            break;
        case (CALLSS):
            CallsS.store(proc1, proc2);
            break;
        default:
            throw invalid_argument("Not a Procedure-Procedure Relation");
    }
}

/*
Retrieve Procedure-Procedure Relationship. For Relation(proc1, proc2)
@param proc Procedure Name
@param var Variable Name
@param type Type of relation
@returns Value of relation stored
*/
bool Storage::retrieveRelation(string proc1, string proc2, ProcProcRelationType type) {
    switch (type) {
        case (CALLS):
            return Calls.retrieve(proc1, proc2);
            break;
        case (CALLSS):
            return CallsS.retrieve(proc1, proc2);
            break;
        default:
            throw invalid_argument("Not a Procedure-Procedure Realtion");
    }
}

/*
Retrieve Forward Relation Stored. For Relation(proc1, proc2)
@param proc Procedure Name
@param type Type of relation
@returns All proc2 such that Relation(proc1, proc2) is True
*/
vector<string> Storage::forwardRetrieveRelation(string proc1, ProcProcRelationType type) {
    switch (type) {
        case (CALLS):
            return Calls.forwardRetrieve(proc1);
            break;
        case (CALLSS):
            return CallsS.forwardRetrieve(proc1);
            break;
        default:
            throw invalid_argument("Not a Procedure-Procedure Relation");
    }
}

/*
Retrieve Reverse Relation Stored. For Relation(proc1, proc2)
@param var Variable Name
@param type Type of relation
@returns All proc1 such that Relation(proc1, proc2) is True
*/
vector<string> Storage::reverseRetrieveRelation(string proc2, ProcProcRelationType type) {
    switch (type) {
        case (USESSV):
            return Calls.reverseRetrieve(proc2);
            break;
        case (MODIFIESSV):
            return CallsS.reverseRetrieve(proc2);
            break;
        default:
            throw invalid_argument("Not a Procedure-Procedure Relation");
    }
}

/**
Only called after filling up Procedure-Procedure, fills in the Star
@param type Type of relation

*/
void Storage::buildStar(ProcProcRelationType type) {
    switch (type) {
        case (CALLS):
            CallsS = Calls.buildStar();
            break;
        default:
            throw invalid_argument("Not a Procedure-Procedure Realtion");
    }
}

/*
Compute Forward Relation Stored. For Next(stmt1, stmt2) or Affects(stmt1,stmt2)
@param stmt lineNum
@param type Type of relation
@returns All stmt2 such that Relation(stmt, stmt2) is True
*/
vector<int> Storage::forwardComputeRelation(int stmt, StmtStmtRelationType type) {
    shared_ptr<CFGNode> cfgNode = this->CFGMap->at(stmt);
    vector<int> lstLineNum = {};

    switch (type) {
        case (NEXT): {
            for (const auto &childNode: cfgNode->getChildren()) {
                shared_ptr<TNode> tNode = childNode->getTNode();

                // normal child node
                if (dynamic_pointer_cast<Statement>(tNode) != nullptr) {
                    shared_ptr<Statement> stmt = dynamic_pointer_cast<Statement>(tNode);
                    lstLineNum.push_back(stmt->getLineNum());

                    // childNode is a dummy node
                } else if (tNode == nullptr && !childNode->getChildren().empty()) {
                    for (const auto &storedChildNode: childNode->getChildren()) {
                        shared_ptr<TNode> storedChildTNode = storedChildNode->getTNode();

                        if (dynamic_pointer_cast<Statement>(storedChildTNode) != nullptr) {
                            shared_ptr<Statement> stmt = dynamic_pointer_cast<Statement>(storedChildTNode);
                            lstLineNum.push_back(stmt->getLineNum());
                        }
                    }
                }
            }
            return lstLineNum;
        }

        case (NEXTS): {
            // reset visited
            shared_ptr<map<int, bool >> visited = make_shared<map<int, bool >>();

            vector<int> childrenLineNums = this->getNextStarForwardLineNum(cfgNode, visited);
            lstLineNum.insert(lstLineNum.end(), childrenLineNums.begin(), childrenLineNums.end());

            return lstLineNum;
        }
        case(AFFECTS): {
            // Check if both are affects
            shared_ptr<AssignStatement> stmtNode = dynamic_pointer_cast<AssignStatement>(statements[stmt]);

            if (stmtNode == nullptr) {
                return {};
            }

            // Check CFG path + if variable 
            shared_ptr<CFGNode> cfgNode = this->CFGMap->at(stmt);

            shared_ptr<set<pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>>> visited = make_shared<set<pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>>>();
            string var = stmtNode->getVarName();

            set<int> result = forwardAffectsHelper(cfgNode, nullptr, var, visited);
            vector<int> output = {};
            copy(result.begin(), result.end(), output.begin());
            return output;
        }
        case(AFFECTSS): {
            shared_ptr<AssignStatement> stmtNode = dynamic_pointer_cast<AssignStatement>(statements[stmt]);

            // Check if current statement is assign
            if (stmtNode == nullptr) {
                return {};
            }


            shared_ptr<CFGNode> cfgNode = this->CFGMap->at(stmt);

            unordered_map<int, bool> visited = {};
            queue<int> nodeQueue;
            nodeQueue.push(stmt);

            vector<int> output = {};

            while (!nodeQueue.empty()) {
                // Get next item in queue
                int currStmt = nodeQueue.front();
                nodeQueue.pop();

                // Skip if visited before
                if (visited[currStmt]) {
                    continue;
                }

                // Mark as visited and add to result
                visited[currStmt] = true;
                output.push_back(currStmt);

                // Get all statement that this affects
                vector<int> nextResult = forwardComputeRelation(currStmt, AFFECTS);

                // Add all next result into queue
                for (int x : nextResult) {
                    nodeQueue.push(x);
                }

            }

            return output;
        }
        default:
            throw invalid_argument("Not a Statement-Statement Relation");
    }
}

/**
Helper function for retrieve affects
@param currNode Current node to check
@param parentNode Parent node of the current node
@param var Variable to check for
@param visited Visited set of (current, parent) to prevent infinite loops
@returns Value of relation stored
*/
set<int> Storage::forwardAffectsHelper(shared_ptr<CFGNode> currNode, shared_ptr<CFGNode> parentNode, string var, shared_ptr<set<pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>>> visited) {
    set<int> result = {};

    // Check if path visited before
    if (visited->find(pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>(currNode, parentNode)) == visited->end()) {
        return result;
    }
    visited->insert(pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>(currNode, parentNode));

    // If blank node, recurse into childNodes
    shared_ptr<Statement> statement_node = dynamic_pointer_cast<Statement>(currNode->getTNode());
    if (statement_node == nullptr) {
        for (const auto& childNode : currNode->getChildren()) {
            set<int> childResult = forwardAffectsHelper(childNode, currNode, var, visited);
            result.insert(childResult.begin(), childResult.end());
        }
        return result;
    }

    // Check if current node is assign and uses var -> Add current node
    int lineNo = statement_node->getLineNum();
    shared_ptr<AssignStatement> assign_node = dynamic_pointer_cast<AssignStatement>(statement_node);
    if (assign_node != nullptr && retrieveRelation(lineNo, var, USESSV) && parentNode != nullptr) {
        result.insert(lineNo);
    }
    
    // If current statement modifies var -> end recursion
    if (retrieveRelation(lineNo, var, MODIFIESSV)) {
        return result;
    }

    // Recurse to child nodes
    for (const auto& childNode : currNode->getChildren()) {
        set<int> childResult = forwardAffectsHelper(childNode, currNode, var, visited);
        result.insert(childResult.begin(), childResult.end());
    }
    return result;
}

/**
 * helper function to recursively get all line numbers of child nodes in CFGNode
 * @param node
 * @return lines numbers of all child nodes(recursively) where Next*(n1, n2)
 */
vector<int> Storage::getNextStarForwardLineNum(shared_ptr<CFGNode> node, shared_ptr<map<int, bool >> visited) {
    vector<int> lstLineNum = {};

    // add children
    for (const auto &childNode: node->getChildren()) {
        shared_ptr<TNode> TNode = childNode->getTNode();

        if (dynamic_pointer_cast<Statement>(TNode) != nullptr) {
            shared_ptr<Statement> stmt = dynamic_pointer_cast<Statement>(TNode);
            int lineNum = stmt->getLineNum();

            // IF FOUND
            if (visited->find(lineNum) != visited->end()) {
                continue;
            } else {
                // add children stmt to list
                visited->insert({lineNum, true});
                lstLineNum.push_back(lineNum);

                //recursively get child nodes
                vector<int> childrenLineNums = getNextStarForwardLineNum(childNode, visited);
                lstLineNum.insert(lstLineNum.end(), childrenLineNums.begin(), childrenLineNums.end());
            }
        } else if (TNode == nullptr && !childNode->getChildren().empty()) {
            //recursively get children nodes
            vector<int> childrenLineNums = getNextStarForwardLineNum(childNode, visited);
            lstLineNum.insert(lstLineNum.end(), childrenLineNums.begin(), childrenLineNums.end());
        }
    }
    return lstLineNum;
}

/*
Compute Backward Relation Stored. For Next(stmt1, stmt2) or Affects(stmt1,stmt2)
@param stmt lineNum
@param type Type of relation
@returns All stmt1 such that Relation(stmt1, stmt) is True
*/
vector<int> Storage::backwardComputeRelation(int stmt, StmtStmtRelationType type) {
    shared_ptr<CFGNode> cfgNode = this->CFGMap->at(stmt);
    vector<int> lstLineNum = {};
    switch (type) {
        case (NEXT): {
            for (const auto& parentNode : cfgNode->getParents()) {
                shared_ptr<TNode> tNode = parentNode->getTNode();

                if (dynamic_pointer_cast<Statement>(tNode) != nullptr) {
                    shared_ptr<Statement> stmt = dynamic_pointer_cast<Statement>(tNode);
                    lstLineNum.push_back(stmt->getLineNum());

                }
                else if (parentNode->getTNode() == nullptr && !parentNode->getParents().empty()) {
                    for (const auto& storedParentNode : parentNode->getParents()) {
                        shared_ptr<TNode> storedParentTNode = storedParentNode->getTNode();

                        if (dynamic_pointer_cast<Statement>(storedParentTNode) != nullptr) {
                            shared_ptr<Statement> stmt = dynamic_pointer_cast<Statement>(storedParentTNode);
                            lstLineNum.push_back(stmt->getLineNum());
                        }
                    }
                }
            }
            return lstLineNum;
        }
        case (NEXTS): {
            // reset visited
            shared_ptr<map<int, bool >> visited = make_shared<map<int, bool >>();

            vector<int> parentLineNums = this->getNextStarBackwardLineNum(cfgNode, visited);
            lstLineNum.insert(lstLineNum.end(), parentLineNums.begin(), parentLineNums.end());

            return lstLineNum;
        }
        case (AFFECTS): {
            // Check if statement is assign
            shared_ptr<AssignStatement> stmtNode = dynamic_pointer_cast<AssignStatement>(statements[stmt]);

            if (stmtNode == nullptr) {
                return {};
            }

            // Check CFG path + variables
            shared_ptr<CFGNode> cfgNode = this->CFGMap->at(stmt);

            shared_ptr<set<pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>>> visited = make_shared<set<pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>>>();

            vector<string> temp = forwardRetrieveRelation(stmt, USESSV);
            set<string> var = {};

            for (auto x : temp) {
                var.insert(x);
            }

            set<int> result = reverseAffectsHelper(cfgNode, nullptr, var, visited);
            vector<int> output = {};
            copy(result.begin(), result.end(), output.begin());
            return output;
        }
        default:
            throw invalid_argument("Not a Statement-Statement Relation");
    }
}

/**
Helper function for retrieve affects
@param currNode Current node to check
@param parentNode Parent node of the current node
@param targetNode Target node to ccheck for
@param var Variable to check for
@param visited Visited set of (current, parent) to prevent infinite loops
@returns Value of relation stored
*/
set<int> Storage::reverseAffectsHelper(shared_ptr<CFGNode> currNode, shared_ptr<CFGNode> childNode, set<string> varsUsed, shared_ptr<set<pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>>> visited) {
    set<int> result = {};

    if (varsUsed.empty()) {
        return result;
    }

    // Check if path visited before
    if (visited->find(pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>(currNode, childNode)) == visited->end()) {
        return result;
    }
    visited->insert(pair<shared_ptr<CFGNode>, shared_ptr<CFGNode>>(currNode, childNode));

    // If blank node, recurse into parentNodes
    shared_ptr<Statement> statement_node = dynamic_pointer_cast<Statement>(currNode->getTNode());
    if (statement_node == nullptr) {
        for (const auto& parentNode : currNode->getParents()) {
            set<int> childResult = reverseAffectsHelper(parentNode, currNode, varsUsed, visited);
            result.insert(childResult.begin(), childResult.end());
        }
        return result;
    }

    // Check if current node is assign and modifes any var -> Add current node to output
    // If childNode is null -> still at first step, dont add itself
    int lineNo = statement_node->getLineNum();
    shared_ptr<AssignStatement> assignNode = dynamic_pointer_cast<AssignStatement>(statement_node);
    if (assignNode != nullptr && childNode != nullptr) {
        string varModify = assignNode->getVarName();
        if (varsUsed.find(varModify) != varsUsed.end()) {
            result.insert(lineNo);
        }
    }

    // If current statement modifies any vars used -> remove
    vector<string> varsModified = forwardRetrieveRelation(lineNo, MODIFIESSV);
    for (auto x : varsModified) {
        varsUsed.erase(x);
    }

    // Recurse to parent nodes
    for (const auto& parentNode : currNode->getParents()) {
        set<int> childResult = reverseAffectsHelper(parentNode, currNode, varsUsed, visited);
        result.insert(childResult.begin(), childResult.end());
    }
    return result;
}


/**
 * helper function to recursively get all line numbers of parent nodes in CFGNode
 * @param node
 * @return lines numbers of all parent nodes(recursively) where Next*(n1, n2)
 */
vector<int> Storage::getNextStarBackwardLineNum(shared_ptr<CFGNode> node, shared_ptr<map<int, bool >> visited) {
    vector<int> lstLineNum = {};

    if (dynamic_pointer_cast<Statement>(node->getTNode()) != nullptr) {
        shared_ptr<Statement> stmt = dynamic_pointer_cast<Statement>(node->getTNode());
    }


    for (const auto &parentNode: node->getParents()) {
        shared_ptr<TNode> tNode = parentNode->getTNode();

        if (dynamic_pointer_cast<Statement>(tNode) != nullptr) {
            shared_ptr<Statement> stmt = dynamic_pointer_cast<Statement>(tNode);
            int lineNum = stmt->getLineNum();

            // IF FOUND
            if (visited->find(lineNum) != visited->end()) {
                continue;
            } else {
                // add parent stmt to list
                visited->insert({lineNum, true});
                lstLineNum.push_back(lineNum);

                //recursively get parent nodes
                vector<int> parentLineNums = getNextStarBackwardLineNum(parentNode, visited);
                lstLineNum.insert(lstLineNum.end(), parentLineNums.begin(), parentLineNums.end());
            }
            // node is dummy node
        } else if (tNode == nullptr && !parentNode->getParents().empty()) {
            //recursively get parent nodes
            vector<int> parentLineNums = getNextStarBackwardLineNum(parentNode, visited);
            lstLineNum.insert(lstLineNum.end(), parentLineNums.begin(), parentLineNums.end());
        }
    }
    return lstLineNum;
}