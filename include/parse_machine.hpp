#pragma once
#include <string>
#include <vector>

enum class Opcode {
  MATCH,
  JUMP,
  HALT,

  //Internal instruction appended to end of instruction array
  OUT_OF_BOUNDS, //Semantically same as HALT
  UNSET
};

enum class Runtype{
    NONE,
    MATCH,
    PARSE,
    SEARCH
};

//allow for easier update of instruction argument sizes later if needed
typedef char mchar;
typedef unsigned long maddress;

typedef union Argument {
  mchar c;
  maddress address;
  Argument(mchar input);
  Argument(maddress input);
  Argument();
} Argument;

typedef struct Instruction {
  public:
    Opcode opcode;
    Argument argument1;
    Argument argument2;
    Instruction();
    Instruction(Opcode code_type);
    Instruction(Opcode code_type, mchar char_input, maddress int_input);
    Instruction(Opcode code_type, maddress int_input);
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

std::string GetOpcodeString (Opcode opcode);