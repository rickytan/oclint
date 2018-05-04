#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ObjCMethodDeclStyleRule : public oclint::AbstractASTVisitorRule<ObjCMethodDeclStyleRule> {

public:
    virtual const string name() const override {
        return "objective-c method declation style";
    }

    virtual int priority() const override {
        return 3;
    }

    virtual const string category() const override {
        return "style";
    }

    bool VisitObjCMethodDecl(ObjCMethodDecl *D) {
        SourceManager &sm = _carrier->getSourceManager();
        
        auto start = D->getLocStart();
        // +/- 前面需要回车
        auto pre = start.getLocWithOffset(-1);
        if (pre.isValid()) {
            const char *code = sm.getCharacterData(pre);
            if (code && code[0] != '\n') {
                addViolation(D, this, "方法声明 +/- 前需要回车");
            }
        }
        
        // +/- 后面需要空格
        auto post = start.getLocWithOffset(1);
        if (post.isValid()) {
            const char *code = sm.getCharacterData(post);
            if (code && code[0] != ' ') {
                addViolation(D, this, "方法声明 +/- 后需要空格");
            }
        }
        return true;
    }
};

static RuleSet rules(new ObjCMethodDeclStyleRule);
