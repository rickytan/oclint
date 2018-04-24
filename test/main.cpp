
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <clang/Tooling/Tooling.h>

#include "oclint/RuleBase.h"
#include "rules/style/CompoundStatementStyleRule.cpp"

using namespace std;
using namespace clang;
using namespace oclint;

class TestProcessor : public ASTConsumer
{
private:
    RuleBase* _rule;
    ViolationSet *_violationSet;
    
public:
    TestProcessor(RuleBase *rule, ViolationSet *violationSet)
    {
        _rule = rule;
        _violationSet = violationSet;
    }
    
    virtual void HandleTranslationUnit(ASTContext &astContext) override
    {
        RuleCarrier *carrier = new RuleCarrier(&astContext, _violationSet);
        _rule->takeoff(carrier);
    }
};

class TestFrontendAction : public clang::ASTFrontendAction
{
private:
    RuleBase* _rule;
    ViolationSet *_violationSet;
    
public:
    TestFrontendAction(RuleBase *rule, ViolationSet *violationSet)
    {
        _rule = rule;
        _violationSet = violationSet;
    }
    
    std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef) override
    {
        return llvm::make_unique<TestProcessor>(_rule, _violationSet);
    }
};

/*
static void dump_stmt(Stmt *stmt) {
    string dump;
    llvm::raw_string_ostream ss(dump);
    stmt->printPretty(ss, nullptr, PrintingPolicy(LangOptions()));
    ss.flush();
    cout << dump << endl;
}
 */

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " source_file.cpp" << std::endl;
        return 1;
    }
    
    string filepath(argv[1]);
    ifstream ifs(filepath);
    if (ifs.is_open()) {
        stringstream buffer;
        buffer << ifs.rdbuf();
        string code(buffer.str());
        
        ViolationSet violationSet;
        if (!tooling::runToolOnCodeWithArgs(new TestFrontendAction(new CompoundStatementStyleRule(), &violationSet),
                                           llvm::Twine(code), {"-c", "-fsyntax-only"}, filepath)) {
            for (auto v : violationSet.getViolations()) {
                cout << v.path << ":" << v.startLine << ":" << v.startColumn << ": " << v.message << endl;
            }
        }
    }
    else {
        std::cout << "File: " << argv[1] << " not exist!" << std::endl;
        return 1;
    }
    return 0;
}
