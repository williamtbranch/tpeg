#include <iostream>
#include <vector>
#include <string>

class gobble {
    public:
        std::string content;
        int consumed;
        bool status;
        gobble (std::string parentString, int index);
};


class pegExpression {
    std::string expression;
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

class grammarTest{
    public:
        grammarDef grammar;
        bool isValid;
        std::vector<testCase> tests; 
        grammarTest(const std::string &parseString, int index);
};

class testBatch {
    public:
        std::vector<grammarTest> batch;
        testBatch(std::string inputTests);
};