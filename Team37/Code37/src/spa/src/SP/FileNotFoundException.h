#ifndef SPA_FILENOTFOUNDEXCEPTION_H
#define SPA_FILENOTFOUNDEXCEPTION_H

using namespace std;

#include <stdexcept>

class FileNotFoundException : public std::exception {
private:
    char * message;

public:
    FileNotFoundException(char * msg) : message(msg) {}
    char * what () {
        return message;
    }
};

#endif //SPA_FILENOTFOUNDEXCEPTION_H
