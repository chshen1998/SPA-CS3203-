#ifndef SPA_CONDITIONALEXPRESSIONNODE_H
#define SPA_CONDITIONALEXPRESSIONNODE_H

using namespace std;

#include <vector>

class ConditionalExpressionNode {
private:
    vector<int> bracketIndexes;
    vector<shared_ptr<ConditionalExpressionNode>> children;
    shared_ptr<ConditionalExpressionNode> parent;
public:
    ConditionalExpressionNode(
            vector<int> bracketIndexes,
            vector<shared_ptr<ConditionalExpressionNode>> children,
            shared_ptr<ConditionalExpressionNode> parent
    ) : bracketIndexes(bracketIndexes), children(children), parent(parent){}

    vector<shared_ptr<ConditionalExpressionNode>> getChildren() {
        return children;
    }

    shared_ptr<ConditionalExpressionNode> getParent() {
        return parent;
    }

    void setParent(shared_ptr<ConditionalExpressionNode> newParent) {
        parent = newParent;
    }

    void setChild(shared_ptr<ConditionalExpressionNode> child) {
        children.push_back(child);
    }
};


#endif //SPA_CONDITIONALEXPRESSIONNODE_H
