#include <parse_machine.hpp>
#include <vector>
#include <string>
#include <iostream>

Argument::Argument (char input) {
  c = input;
}

Argument::Argument (unsigned int input) {
  address = input;
}


Instruction::Instruction(Opcode input_opcode, Argument input_argument)
  :opcode(input_opcode), argument(input_argument) {

}

Instruction::Instruction(Opcode input_opcode, char input_argument)
  :opcode(input_opcode), argument(input_argument){

}

Instruction::Instruction(Opcode input_opcode, unsigned int input_argument)
  :opcode(input_opcode), argument(input_argument){

}

//Some Instructions don't need arguments
Instruction::Instruction(Opcode input_opcode)
  :opcode(input_opcode), argument(char(0)){

}

ParseMachine::ParseMachine(){
  SetParseString("");
  std::vector<Instruction> temp_code;
  SetParseCode(temp_code);
  
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

  bool validity {true}; //Final result of run

  bool exit {false};
  while (!exit){
    switch (code_[pc].opcode)
    {
      case Opcode::HALT:
        match_ = validity;
        return;
        break;
      case Opcode::MATCH:
        if (code_[pc].argument.c != parse_string_[c]){
          match_ = false;
          return;
        }
        pc++;
        c++;
        break;
      case Opcode::OUT_OF_BOUNDS:
        match_ = validity;
        return;
        break;
      default:
        break;
    }
  }
}