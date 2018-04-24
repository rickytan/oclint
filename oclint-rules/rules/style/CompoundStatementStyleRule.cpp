#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class CompoundStatementStyleRule : public oclint::AbstractASTVisitorRule<CompoundStatementStyleRule> {
public:
    virtual const string name() const override
    {
        return "compound statement style";
    }
    
    virtual int priority() const override
    {
        return 3;
    }
    
    virtual const string category() const override
    {
        return "style";
    }
    
    bool VisitCompoundStmt(Stmt *stmt) {
        auto start = stmt->getLocStart();
        auto end = stmt->getLocEnd();
        
        clang::SourceManager *sourceManager = &_carrier->getSourceManager();
        
        auto prestart = start.getLocWithOffset(-1);
        if (prestart.isValid()) {
            const char *code = sourceManager->getCharacterData(prestart);
            if (!(code[0] == '\n' ||
                  code[0] == '\t' ||
                  code[0] == ' ' ||
                  code[0] == '(')) {
                addViolation(stmt, this, "左大括号前需要空格或回车");
            }
        }
        auto postend = end.getLocWithOffset(1);
        if (postend.isValid()) {
            const char *code = sourceManager->getCharacterData(postend);
            if (!(code[0] == '\n' ||
                  code[0] == '\t' ||
                  code[0] == ' ' ||
                  code[0] == ')')) {
                addViolation(end, postend, this, "右大括号后需要空格或回车");
            }
        }
        return true;
    }
};

static RuleSet rules(new CompoundStatementStyleRule());
