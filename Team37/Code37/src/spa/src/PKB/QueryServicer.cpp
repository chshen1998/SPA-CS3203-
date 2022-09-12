#include "QueryServicer.h"


QueryServicer::QueryServicer(shared_ptr<Storage> storage) {
	this->storage = storage;
}

// Retrieve all of an element
// TEMP: Return all variables only
// GOAL: Take in element type and return correct type
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