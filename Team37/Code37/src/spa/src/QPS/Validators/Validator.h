#pragma once

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>

#include "../QPS.h"

#ifndef TEAM37_VALIDATOR_H
#define TEAM37_VALIDATOR_H


class Validator
{
protected:
	Validator();

	void updatePqlError(ErrorType type, string msg);

	PqlError pe;
};

#endif //TEAM37_VALIDATOR_H
