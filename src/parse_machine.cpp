#include <parse_machine.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

Argument::Argument (bool input):flag {input} {
}

Argument::Argument (char input):c {input} {
}

Argument::Argument (unsigned long input):address {input} {
}

Argument::Argument (int input):address {(unsigned long int)input} {
}

Argument::Argument ():address {0} {
}

Instruction::Instruction(){
  opcode = Opcode::UNSET;
}

Instruction::Instruction(Opcode code_type):opcode {code_type}
{
  switch (code_type)
  {
  case Opcode::HALT:
  case Opcode::OUT_OF_BOUNDS:
  case Opcode::UNSET:
    break;
  
  default:
    std::cerr << "ERROR: Wrong instruction initializer used for opcode "
      << GetOpcodeString(code_type) << "." << std::endl;
    abort();
    break;
  }
}

Instruction::Instruction(
  Opcode code_type,
  Argument input_1,
  Argument input_2)
    :opcode {code_type}, argument1 {input_1}, argument2 {input_2} 
{
  switch (code_type)
  {
  case Opcode::MATCH:
    break;
  
  default:
    std::cerr << "ERROR: Wrong instruction initializer used for opcode "
      << GetOpcodeString(code_type) << "." << std::endl;
    abort();
    break;
  }
}

Instruction::Instruction(
  Opcode code_type,
  Argument input_1)
    :opcode {code_type}, argument1 {input_1} 
{
  switch (code_type)
  {
  case Opcode::JUMP:
  case Opcode::SET_VALID:
    break;
  
  default:
    std::cerr << "ERROR: Wrong instruction initializer used for opcode "
      << GetOpcodeString(code_type) << "." << std::endl;
    abort();
    break;
  }
}

ParseMachine::ParseMachine(){
  SetParseString("");
  std::vector<Instruction> empty_code;
  SetParseCode(empty_code);
}

ParseMachine::ParseMachine(std::vector<Instruction> input_code,
  std::string input_string)
{
  SetParseString(input_string);
  SetParseCode(input_code);
}

std::string ParseMachine::GetParseString(){
  std::string output_string;
  output_string.assign(parse_string_);
  output_string.pop_back();
  return output_string;
}

void ParseMachine::SetParseString(std::string input_string){
  parse_string_.clear();
  parse_string_.assign(input_string);
  parse_string_.push_back(0);
}

std::vector<Instruction> ParseMachine::GetParseCode(){
  std::vector<Instruction> return_code;
  for (int i=0; i < code_.size(); i++){
    return_code.push_back(code_[i]);
  }
  while(return_code.size() > 0
    && return_code[return_code.size()-1].opcode == Opcode::OUT_OF_BOUNDS){
    return_code.pop_back();
  }
  return return_code;
}

void ParseMachine::SetParseCode(std::vector<Instruction> input_code){
  code_.clear();
  for (int i=0; i < input_code.size(); i++){
    code_.push_back(input_code[i]);
  }

  //Make sure to prevent the program counter moving out of bounds
  //during run.
  if(code_.size() > 0){ 
    switch (code_[code_.size()-1].opcode)
    {
      //Out of bounds instructions not for user code
      //replace with HALT when found at tail
      case Opcode::OUT_OF_BOUNDS:
        while(code_.size() > 0
          && code_[code_.size()-1].opcode == Opcode::OUT_OF_BOUNDS)
        {
          code_.pop_back();
        }
        code_.push_back(Instruction(Opcode::HALT));
        break;

      case Opcode::HALT:
      case Opcode::JUMP: //unconditional jump is safe
        break;

      default:
        code_.push_back(Instruction(Opcode::OUT_OF_BOUNDS));
        break;
    }
  }
  else {
    code_.push_back(Instruction(Opcode::OUT_OF_BOUNDS));
  }
}

void ParseMachine::Set(std::vector<Instruction> input_code,
  std::string input_string)
{
  SetParseString(input_string);
  SetParseCode(input_code);
}

bool ParseMachine::Match(){
  runtype_ = Runtype::MATCH;
  Run();
  return match_;
}

void ParseMachine::Run(){
  //machine registers
  int pc {0}; //program counter
  long c {0}; //character index

  bool validity {false}; //Final result of run

  bool exit {false};
  while (!exit){
    switch (code_[pc].opcode)
    {
      case Opcode::HALT:
        match_ = validity;
        return;
        break;
      case Opcode::MATCH:
        if (code_[pc].argument1.c != parse_string_[c]){
          pc=code_[pc].argument2.address;
        }
        else{
          c++;
          pc++;
        }
        break;
      case Opcode::OUT_OF_BOUNDS:
        match_ = validity;
        return;
        break;
      case Opcode::SET_VALID:
        validity = code_[pc].argument1.flag;
        pc++;
        break;
      case Opcode::JUMP:
        pc = code_[pc].argument1.address;
        break;
      default:
        std::cerr << "ERROR: Undefined opcode: instruction escaped dispatch" <<
          std::endl;
        abort();
        break;
    }
  }
}

std::string GetOpcodeString (Opcode opcode){
  switch (opcode)
  {
  case Opcode::MATCH:
    return "MATCH";
    break;
  case Opcode::JUMP:
    return "JUMP";
    break;
  case Opcode::HALT:
    return "HALT";
    break;
  case Opcode::OUT_OF_BOUNDS:
    return "OUT_OF_BOUNDS";
    break;
  case Opcode::UNSET:
    return "UNSET";
    break;
  case Opcode::SET_VALID:
    return "SET_VALID";
    break;
  
  default:
    return "UNREGISTERED OPCODE";
    break;
  }
}
