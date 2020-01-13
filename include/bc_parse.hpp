#pragma once
#include <string>
#include <parse_machine.hpp>


//byte code parsing functions

//Helper functions for instruction generation
std::vector<Instruction> Parse (std::string input_string);
Instruction ParseInstruction (std::string input_string);
Opcode GetOpcodeFromName(std::string opcode_name);