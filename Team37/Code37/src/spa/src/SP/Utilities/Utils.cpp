#include "Utils.h"
#include "Keywords.h"
#include "../InvalidSyntaxException.h"

const string Utils::WHITESPACE = " \n\r\t\f\v";

string Utils::fileToString(string filepath) {
    string s;
    string sTotal;

    ifstream in;
    in.open(filepath);

    if (in.is_open()) {
        while(!in.eof()) {
            getline(in, s);
            // TODO: investigate if "\n" is needed
            sTotal += s + "\n";
        }
        in.close();
    } else {
        // TODO: Throw error
        cout << "Unable to open file." << endl;
    }

    return sTotal;
}

string Utils::ltrim(string s) {
    size_t start = s.find_first_not_of(Utils::WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string Utils::rtrim(string s) {
    size_t end = s.find_last_not_of(Utils::WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string Utils::trim(string s) {
    return rtrim(ltrim(s));
}

bool Utils::validateName(string varName) {
    // Variable name cannot be empty string
    if (varName.length() == 0) {
        throw InvalidSyntaxException((char *) "Some variable name is empty");
    }

    char currChar;

    // First character must be a letter "A-Z" || "a-z"
    currChar = varName[0];
    if (!isalpha(currChar)) {
        throw InvalidSyntaxException((char *) "First character of variable name must be an alphabet");
    }

    varName.erase(0);

    for (char& c: varName)  {
        if (!(isalpha(c) || isdigit(c))) {
            throw InvalidSyntaxException((char *) "Variable can only contain alphanumerical values");
        }
    }
    return true;
}

bool Utils::validateInteger(string integer) {
    // Variable name cannot be empty string
    if (integer.length() == 0) {
        throw InvalidSyntaxException((char *) "Some variable name is empty");
    }

    for (char& c: integer)  {
        if (!isdigit(c)) {
            throw InvalidSyntaxException((char *) "First character of variable name must be an alphabet");
        }
    }

    return true;
}
