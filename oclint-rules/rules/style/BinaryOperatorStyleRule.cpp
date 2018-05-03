#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class BinaryOperatorStyleRule : public oclint::AbstractASTVisitorRule<BinaryOperatorStyleRule> {
public:
    virtual const string name() const override {
        return "binary operator style";
    }

    virtual int priority() const override {
        return 3;
    }

    virtual const string category() const override {
        return "style";
    }

    bool VisitBinaryOperator(clang::BinaryOperator *S) {
        SourceManager &sm = _carrier->getSourceManager();
    
        auto start = S->getOperatorLoc();
        auto end = start.getLocWithOffset(S->getOpcodeStr().size() - 1);

        // 双目操作符后面需要空格或回车
        auto post = end.getLocWithOffset(1);
        if (post.isValid()) {
            const char *code = sm.getCharacterData(post);
            if (code && string(" \n").find(code[0]) == string::npos) {
                addViolation(start, end, this, "双目操作符后需要空格");
            }
        }
        
        // 双目操作符前面需要空格或回车
        auto pre = start.getLocWithOffset(-1);
        if (pre.isValid()) {
            const char *code = sm.getCharacterData(pre);
            if (code && string(" \n").find(code[0]) == string::npos) {
                addViolation(start, end, this, "双目操作符前需要空格");
            }
        }
        return true;
    }
};

static RuleSet rules(new BinaryOperatorStyleRule);
