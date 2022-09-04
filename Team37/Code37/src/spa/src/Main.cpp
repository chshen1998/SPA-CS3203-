#include <iostream>

using namespace std;

#include "SP/SP.h"

int main(int argc, char** argv) {
    SP sp;
    string file = "";
    if( argc == 2 ) {
        file = argv[1];
        sp.parse(file);
    }
    else {
        cout << "Usage: ./main InputFile\n";
        return 1;
    }
}

