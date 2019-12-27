#include <iostream>
#include <vector>
#include <string>

enum class ErrorType{
    NON_ERROR,
    UNKNOWN,
    BAD_INPUT,
    EMPTY_LIST,
    UNBALANCED, //unbalanced parens
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
    public:
        std::string name;
        std::string rule;
        pegExpression (const std::string& parseString);
};

class grammarDef{
    public:
        std::vector<pegExpression> expressions;
        grammarDef(const std::string& parseString);
        grammarDef();
};

enum class testType {
    MATCH,
    SEARCH
};

class testCase{
    public:
        testType type;
        std::string input;
        std::string expected;
        testCase(const std::string& parseString);
};

class grammarTest{
    public:
        grammarDef grammar;
        error grammarTestError;
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