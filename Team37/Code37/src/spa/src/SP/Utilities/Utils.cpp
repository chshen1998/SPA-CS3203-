#include "Utils.h"
#include "Keywords.h"

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
