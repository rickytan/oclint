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

    /**
     如果是 Block 声明，后面允许 ';'。如：
     @code
     id block = ^(NSString * text) {};
     @endcode
     
     如果 Block 做为消息发送的最后一个参数，后面允许 ']'。如：
     @code
     [self someMethods:^() {
     
     }];
     @endcode
     
     如果 Block 做为 C 函数的非最后一个参数，后面允许 ','。如：
     @code
     makeAFunctionCall(^() {
        // block body
     }, YES);
     @endcode
     
     Block 表达式参数为空时，前面允许 '^'。如：
     @code
     id block = ^{};
     @endcode
     
     内联复合表达式允许大括号被包在小括号中，前面允许 '('，后面允许 ')'，如：
     @code
     NSString *string = ({
       NSMutableString *str = [NSMutableString string];
       [str appendString:@"foo"];
       [str appendString:@"bar"];
       [str copy];
     });
     @endcode
     
     @warning 这里只将所有允许的情况列出来，并不做真实类型检查，即并不关心是 If 语句的代码，
              还是方法实现体的代码，等。
     */
    bool VisitCompoundStmt(CompoundStmt *stmt) {
        clang::SourceManager *sourceManager = &_carrier->getSourceManager();
        
        auto start = stmt->getLBracLoc();
        auto end = stmt->getRBracLoc();
        
        auto prestart = start.getLocWithOffset(-1);
        if (prestart.isValid() && sourceManager->getFileID(prestart) == sourceManager->getFileID(start)) {
            const char *code = sourceManager->getCharacterData(prestart);
            if (code && string("\n\t (^").find(code[0]) == string::npos) {
                addViolation(stmt, this, "左大括号前需要空格或回车");
            }
        }
        auto postend = end.getLocWithOffset(1);
        if (postend.isValid() && sourceManager->getFileID(postend) == sourceManager->getFileID(end)) {
            const char *code = sourceManager->getCharacterData(postend);
            if (code && string("\n\t )];,").find(code[0]) == string::npos) {
                addViolation(end, postend, this, "右大括号后需要空格或回车");
            }
        }
        return true;
    }
};

static RuleSet rules(new CompoundStatementStyleRule());
