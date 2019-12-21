#include <iostream>
#include <vector>
#include <string>


class testCase;
class pegExpression;
class grammarDef;
class grammarTest;
class testBatch;

class testBatch {
    public:
        std::vector<grammarTest> batch;
};

class grammarTest{
    public:
        grammarDef grammar;
        std::vector<testCase> tests; 
};

class grammarDef{
    public:
        std::string grammarName;
        std::vector<pegExpression> expressions;
};

enum class testType {
    MATCH,
    SEARCH
};

class testCase{
    testType type;
    std::string input;
    std::string expected;
};