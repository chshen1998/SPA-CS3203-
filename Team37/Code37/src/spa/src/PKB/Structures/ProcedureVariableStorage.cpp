#include "ProcedureVariableStorage.h"

void ProcedureVariableStorage::store(string procedure, string var) {
	forwardStore.insert(pair<string, string>(procedure, var));
	reverseStore.insert(pair<string, string>(var, procedure));
}

bool ProcedureVariableStorage::retrieve(string procedure, string var) {
	std::pair<MMAPIterator, MMAPIterator> result = forwardStore.equal_range(procedure);
	for (MMAPIterator it = result.first; it != result.second; it++) {
		if (it->second == var) {
			return true;
		}
	}
	return false;
}

vector<string> ProcedureVariableStorage::forwardRetrieve(string procedure) {
	vector<string> output;
	std::pair<MMAPIterator, MMAPIterator> result = forwardStore.equal_range(procedure);
	for (MMAPIterator it = result.first; it != result.second; it++) {
		output.push_back(it->second);
	}
	return output;
}

vector<string> ProcedureVariableStorage::reverseRetrieve(string var) {
	vector<string> output;
	std::pair<MMAPIterator, MMAPIterator> result = reverseStore.equal_range(var);
	for (MMAPIterator it = result.first; it != result.second; it++) {
		output.push_back(it->second);
	}
	return output;
}