#include <iostream>
#include <vector>
#include <string>

enum class ErrorType{
    NON_ERROR,
    UNKNOWN,
    BAD_INPUT,
    EMPTY_LIST,
    OUT_OF_BOUNDS
};

class error {
    public:
        std::string errorMSG;
        ErrorType type;
};

class gobble {
    public:
        error gobble_error;
        std::string content;
        int consumed;
        bool status;
        gobble (const std::string& parentString, int index);
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
        bool valid;
        std::vector<testCase> tests; 
        std::string testName;
        std::string content;
        grammarTest(const std::string& parseString);
};

class testBatch {
    public:
        error batch_error;
        bool valid;
        std::vector<grammarTest> batch;
        testBatch(const std::string& inputTests);
};

//Helper functions here
int ConsumeSpaces (const std::string& parseString, int index);