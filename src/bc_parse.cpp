#include <vector>
#include <string>
#include <parse_machine.hpp>
#include <sstream>

//byte code parsing functions

std::vector<Instruction> Parse (std::string input_string){
  std::istringstream f(input_string);
  std::string line;
  Instruction output_instruction;
  while (std::getline(f, line)){
    //Handle MATCH

  }

}