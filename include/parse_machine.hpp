#pragma once
#include <string>

enum Opcode {
    MATCH,
    JUMP,
    OTHER
};

typedef union Argument {
    char c;
    unsigned int address;
    Argument(char input);
    Argument(unsigned int input);
} Argument;

class Instruction {
    public:
        const Opcode opcode;
        const Argument argument;
        Instruction(Opcode input_opcode, Argument input_argument);
        Instruction(Opcode input_opcode, char input_argument);
        Instruction(Opcode input_opcode, unsigned int input_argument);
};


class ParseMachine {
    public:
        std::string input_string_;
        ParseMachine();
    
};