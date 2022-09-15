#include "QueryServicer.h"


QueryServicer::QueryServicer(shared_ptr<Storage> storage) {
	this->storage = storage;
}

set<NameExpression> QueryServicer::getAllVar() {
	return storage->getAllVar();
}	

set<ConstantExpression> QueryServicer::getAllConst() {
	return storage->getAllConst();
}

set<shared_ptr<Statement>> QueryServicer::getAllStmt(StatementType type) {
	set<shared_ptr<Statement>> allStmt = storage->getAllStmt();
	set<shared_ptr<Statement>> filteredStmt = {};
	set<shared_ptr<Statement>>::iterator itr;
	switch (type) {
	case STATEMENT:
		return allStmt;
		break;
	case ASSIGN:
		copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()), [](shared_ptr<Statement>ptr) {
			return dynamic_pointer_cast<AssignStatement>(ptr)!= nullptr; });

		return filteredStmt;
		break;
	case CALL:
		copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()), [](shared_ptr<Statement>ptr) {
			return dynamic_pointer_cast<CallStatement> (ptr) != nullptr; });
		return filteredStmt;
		break;
	case IF:
		copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()), [](shared_ptr<Statement>ptr) {
			return dynamic_pointer_cast<IfStatement>(ptr) != nullptr; });
		return filteredStmt;
		break;
	case PRINT:
		copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()), [](shared_ptr<Statement>ptr) {
			return dynamic_pointer_cast<PrintStatement>(ptr) != nullptr; });
		return filteredStmt;
		break;
	case READ:
		copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()), [](shared_ptr<Statement>ptr) {
			return dynamic_pointer_cast<ReadStatement>(ptr) != nullptr; });
		
		return filteredStmt;
		break;
	case WHILE:
		copy_if(allStmt.begin(), allStmt.end(), inserter(filteredStmt, filteredStmt.end()), [](shared_ptr<Statement>ptr) {
			return dynamic_pointer_cast<WhileStatement>(ptr) != nullptr; });
		return filteredStmt;
		break;
	default:
		throw invalid_argument("No Statement Type Specified");
		break;
	}
}

/*
Retrieve Reverse Relation Stored. For Relation(stmt1, stmt2)
@param stmt2
@param type Type of relation
@returns All stmt1 such that Relation(stmt1, stmt2) is True
*/
bool QueryServicer::retrieveRelation(int stmt1, int stmt2, StmtStmtRelationType type) {
	return storage->retrieveRelation(stmt1, stmt2, type);
}

/*
Retrieve Forward Relation Stored. For Relation(stmt1, stmt2)
@param stmt1
@param type Type of relation
@returns All stmt2 such that Relatioin(stmt1, stmt2) is True
*/
vector<int> QueryServicer::forwardRetrieveRelation(int stmt1, StmtStmtRelationType type) {
	return storage->forwardRetrieveRelation(stmt1, type);
}

/*
Retrieve Reverse Relation Stored. For Relation(stmt1, stmt2)
@param stmt2
@param type Type of relation
@returns All stmt1 such that Relation(stmt1, stmt2) is True
*/
vector<int> QueryServicer::reverseRetrieveRelation(int stmt2, StmtStmtRelationType type) {
	return storage->reverseRetrieveRelation(stmt2, type);
}