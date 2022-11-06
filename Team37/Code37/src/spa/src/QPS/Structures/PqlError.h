using namespace std;

#include <string>

#include "../Types/ErrorType.h"

#ifndef TEAM37_PQLERROR_H
#define TEAM37_PQLERROR_H

struct PqlError {
    ErrorType errorType;
    string message;

    PqlError(ErrorType type, string msg)
        : errorType(type)
        , message(msg)
    {
    }

    bool operator==(const PqlError& other) const
    {
        return (other.errorType == errorType) && (other.message == message);
    }
};

#endif