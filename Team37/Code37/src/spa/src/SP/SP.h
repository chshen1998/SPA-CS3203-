#include <string>
#include <vector>

using namespace std;

class SP {
public:
    SP();
    vector<string> lines;
    void parse(string filename);
};
