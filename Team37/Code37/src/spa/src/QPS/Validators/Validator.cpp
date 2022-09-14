using namespace std;

#include <string>
#include <unordered_map>

#include "../QPS.h"
#include "Validator.h"

Validator::Validator()
{
	pe.type = ErrorType::NONE;
}

void Validator::updatePqlError(ErrorType type, string msg)
{
    pe.type = type;
    pe.message = msg;
}

