#pragma once
#include <string>
#include <vector>

enum class Opcode {
    MATCH,
    JUMP,
    HALT,

    //Internal instruction appended to end of instruction array
    OUT_OF_BOUNDS, //Semantically same as HALT
    OTHER
};

enum class Runtype{
    NONE,
    MATCH,
    PARSE,
    SEARCH
};

typedef union Argument {
    char c;
    unsigned int address;
    Argument(char input);
    Argument(unsigned int input);
} Argument;

typedef struct Instruction {
    public:
        const Opcode opcode;
        const Argument argument;
        Instruction(Opcode input_opcode, Argument input_argument);
        Instruction(Opcode input_opcode, char input_argument);
        Instruction(Opcode input_opcode, unsigned int input_argument);
        Instruction(Opcode input_opcode);
} Instruction;


class ParseMachine {
    private:
        std::string parse_string_;
        std::vector<Instruction> code_;
        void Run();
        bool match_ {false};
        Runtype runtype_ {Runtype::NONE};
    public:
        ParseMachine();
        ParseMachine(std::vector<Instruction> input_code, 
            std::string input_string);
        std::string GetParseString();
        void SetParseString(std::string input_string);
        std::vector<Instruction> GetParseCode();
        void SetParseCode(std::vector<Instruction> input_code);
        void Set(std::vector<Instruction> input_code, 
            std::string input_string);
        bool Match();
};