#pragma once
#include <string>

enum Opcode {
    MATCH
};

union Argument {
    char c;
    long address;
};

typedef struct Instruction {
    Opcode opcode;
    Argument argument;
} Instruction;


class ExpressionMachine {
    public:
        std::string input_string_;
        ExpressionMachine();
    
};