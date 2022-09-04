#include "Expression.h"
#include <string>

using namespace std;

class NameExpression : public Expression {
private:
    string varName;
public:
    NameExpression(string varName);

    /**
     * Get the name of the variable in the name expression
     *
     * @return a name
     */
    string getVarName();
};
