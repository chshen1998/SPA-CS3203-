#include "StatementVariableStorage.h"

void StatementVariableStorage::store(int stmt, string var) {
	forwardStore.insert(pair<int, string>(stmt, var));
	reverseStore.insert(pair<string, int>(var, stmt));
}

bool StatementVariableStorage::retrieve(int stmt, string var) {
	std::pair<MMAPForwardIterator, MMAPForwardIterator> result = forwardStore.equal_range(stmt);
	for (MMAPForwardIterator it = result.first; it != result.second; it++) {
		if (it->second == var) {
			return true;
		}
	}
	return false;
}

vector<string> StatementVariableStorage::forwardRetrieve(int stmt) {
	vector<string> output;
	std::pair<MMAPForwardIterator, MMAPForwardIterator> result = forwardStore.equal_range(stmt);
	for (MMAPForwardIterator it = result.first; it != result.second; it++) {
		output.push_back(it->second);
	}
	return output;
}

vector<int> StatementVariableStorage::reverseRetrieve(string var) {
	vector<int> output;
	std::pair<MMAPReverseIterator, MMAPReverseIterator> result = reverseStore.equal_range(var);
	for (MMAPReverseIterator it = result.first; it != result.second; it++) {
		output.push_back(it->second);
	}
	return output;
}