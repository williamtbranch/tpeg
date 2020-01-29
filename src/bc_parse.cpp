#include <vector>
#include <string>
#include <parse_machine.hpp>
#include <bc_parse.hpp>
#include <sstream>
#include <iostream>
#include <regex>

//byte code parsing functions

std::vector<Instruction> Parse (std::string input_string){
  std::istringstream f(input_string);
  std::string line;
  std::vector<Instruction> code;
  std::smatch matches;
  while (std::getline(f, line)){
    std::regex pattern {R"(^[ \t]*$)"};
    if(!(std::regex_match(line, pattern))){
      code.push_back(ParseInstruction(line));
    }
  }
  return code;
}

Instruction ParseInstruction (std::string input_string){
  //Get tokens
  std::string opcode_string;
  std::string arg1_string {""};
  std::string arg2_string {""};

  std::smatch matches;
  //patter "Opcode arg1 arg2";
  std::regex pattern {R"(^[ \t]*(\w*))"};
  std::regex_search(input_string, matches, pattern);
  opcode_string = matches[1];
  pattern = R"(^[ \t]*\w*[ \t](\w*))";
  if (std::regex_search(input_string, matches, pattern)){
    arg1_string = matches[1];
  }
  pattern = R"(^[ \t]*\w*[ \t]\w*[ \t](\w*))";
  if (std::regex_search(input_string, matches, pattern)){
    arg2_string = matches[1];
  }
  Opcode opcode {GetOpcodeFromName(opcode_string)};
  switch (opcode)
  {
    case Opcode::MATCH:
      if(arg1_string == "" || arg2_string == ""){
        return Instruction(Opcode::UNSET);
      }
      return Instruction(opcode, arg1_string[0], std::stoi(arg2_string));
      break;
    case Opcode::SET_VALID:
    {//protect scope of valid
      if(arg1_string == ""){
        return Instruction(Opcode::UNSET);
      }
      bool valid {false};
      if (arg1_string == "true") valid = true;
      return Instruction(opcode, valid);
    }
      break;
    case Opcode::HALT:
    case Opcode::UNSET:
      return Instruction(opcode);
      break;
    case Opcode::JUMP:
      if(arg1_string == ""){
        return Instruction(Opcode::UNSET);
      }
      return Instruction(opcode, std::stoi(arg1_string));
      break;
    
    default:
      std::cerr << "ERROR: Could not set opcode in ParseInstruction."
        << std::endl;
      break;
  } 
  Instruction hello;
  
  std::vector <std::string> tokens;
  return hello;

}

Opcode GetOpcodeFromName(std::string opcode_name){
  if (opcode_name == "match") return Opcode::MATCH;
  if (opcode_name == "jump") return Opcode::JUMP;
  if (opcode_name == "halt") return Opcode::HALT;
  if (opcode_name == "set_valid") return Opcode::SET_VALID;
  if (opcode_name == "unset") return Opcode::UNSET;
  if (opcode_name == "out_of_bounds") return Opcode::OUT_OF_BOUNDS;
  return Opcode::UNSET; 
}


Shuttle::Shuttle(const std::string &input, std::vector<TreeChar> &input_tree)
  : input_string{input},
    tree{input_tree},
    tree_index{0},
    input_index {0},
    match {true}
{
}

Shuttle bcParseNumber (const Shuttle &shuttle){
  Shuttle bop = shuttle;
  if (bop.input_string[bop.input_index] > '0' 
      && bop.input_string[bop.input_index] <= '9'){
        bop.input_index++;
        return bop;
      }
  bop.match = false;
  return bop;
}