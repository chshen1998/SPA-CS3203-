#ifndef SPA_INVALIDSYNTAXEXCEPTION_H
#define SPA_INVALIDSYNTAXEXCEPTION_H

using namespace std;

#include <stdexcept>

class InvalidSyntaxException : public std::exception {
private:
    char* message;

public:
    InvalidSyntaxException(char* msg)
        : message(msg)
    {
    }
    char* what()
    {
        return message;
    }
};

#endif // SPA_INVALIDSYNTAXEXCEPTION_H
