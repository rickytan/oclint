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

    bool VisitObjCImplementationDecl(ObjCImplementationDecl *D) {
        return true;
    }

    bool VisitObjCMethodDecl(ObjCMethodDecl *D) {
        SourceManager &sm = _carrier->getSourceManager();
        
        auto start = D->getLocStart().getLocWithOffset(1);
        if (start.isValid()) {
            const char *code = sm.getCharacterData(start);
            if (code && code[0] != ' ') {
                addViolation(D, this, "方法声明 +/- 后需要空格");
            }
        }
        return true;
    }
};

static RuleSet rules(new ObjCMethodDeclStyleRule);
