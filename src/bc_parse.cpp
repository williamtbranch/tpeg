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



void PrintTree (const Shuttle &shuttle){
  int index = 0;
  std::string out_string = "";
  while (shuttle.tree_index != index){
    switch (shuttle.tree[index].type)
    {
    case TreeCharType::TREE :
      out_string = shuttle.tree[index].datum.character;
      break;
    case TreeCharType::LENGTH :
      out_string = std::to_string((int)shuttle.tree[index].datum.length);
      break;
    case TreeCharType::RULE :
      switch (shuttle.tree[index].datum.rule)
      {
      case Rule::UNSET_RULE:
        out_string = "unset";
        break;
      case Rule::NUMBER:
        out_string = "number";
        break;
      case Rule::SPACE:
        out_string = "space";
        break;
      case Rule::ID:
        out_string = "id";
        break;
      case Rule::COLON:
        out_string = "colon";
        break;
      case Rule::LABEL:
        out_string = "label";
        break;
      
      default:
        out_string = "****unassigned string label****";
        break;
      }
      break;
    case TreeCharType::ADDRESS :
      out_string = std::to_string((int)shuttle.tree[index].datum.address);
      break;
    
    default:
      std::cout << "Something stupid happened." << std::endl;
      break;
    }
    // std::cout << "At " << index << ": " << out_char << " " << std::endl;
    std::cout <<  out_string << " ";
    index++;
  }
  std::cout << std::endl;
}

Shuttle::Shuttle(const std::string &input, std::vector<TreeChar> &input_tree)
  : input_string{input},
    tree{input_tree},
    tree_index{0},
    input_index {0},
    rule {Rule::UNSET_RULE},
    match {true}
{
  

      tree.resize (input_string.size() * tree_node_size * 2);
}
Shuttle::Shuttle(const Shuttle &input_shuttle, Rule new_rule)
  : input_string{input_shuttle.input_string},
    tree{input_shuttle.tree},
    tree_index{input_shuttle.tree_index},
    input_index {input_shuttle.input_index},
    rule {new_rule},
    match {true}
{
}

Shuttle::Shuttle(const Shuttle &input_shuttle)
  : input_string{input_shuttle.input_string},
    tree{input_shuttle.tree},
    tree_index{input_shuttle.tree_index},
    input_index {input_shuttle.input_index},
    rule {input_shuttle.rule},
    match {input_shuttle.match}
{
}

Shuttle& Shuttle::operator=(const Shuttle &input_shuttle)
{
  tree_index = input_shuttle.tree_index;
  input_index = input_shuttle.input_index;
  match = input_shuttle.match;
  return *this;
}



Shuttle ParseGrammar (Shuttle &shuttle,
  Shuttle (*ParseRule)(Shuttle &shuttle))
  {
    auto tree_index = shuttle.tree_index;
    auto input_index = shuttle.input_index;
    shuttle.tree_index += 4;
    Shuttle bop {ParseRule(shuttle)};
    if (bop.match == true){
      bop.tree[tree_index].type = TreeCharType::TREE;
      bop.tree[tree_index + 1].type = TreeCharType::RULE;
      bop.tree[tree_index + 2].type = TreeCharType::ADDRESS;
      bop.tree[tree_index + 3].type = TreeCharType::LENGTH;
      bop.tree[tree_index].datum.character = '(';
      bop.tree[tree_index + 1].datum.rule = bop.rule;
      bop.tree[tree_index + 2].datum.address = input_index;
      bop.tree[tree_index + 3].datum.length = bop.input_index - input_index;

      bop.tree[bop.tree_index].type = TreeCharType::TREE;
      bop.tree[bop.tree_index].datum.character = ')';
      bop.tree_index++;
      bop.tree[bop.tree_index].type = TreeCharType::TREE_EOF;
    }
    return bop;
  }

//number <- [1-9][0-9]+
Shuttle bcParseNumber (Shuttle &shuttle){
  #define ip bop.input_string[bop.input_index]
  Shuttle bop(shuttle, Rule::NUMBER);
  if (ip > '0' && ip <= '9'){
        bop.input_index++;
      }
  else {
    bop.match = false;
    return bop;
  }
  while (ip >= '0' && ip <= '9'){
    bop.input_index++;
      }
  return bop;
  #undef ip
}

//space  <- [\s\t]+
Shuttle bcParseSpace (Shuttle &shuttle){
  #define ip bop.input_string[bop.input_index]
  Shuttle bop(shuttle, Rule::SPACE);
  if ( ip == ' ' || ip == '\t') bop.input_index++; 
  else {
    bop.match = false;
    return bop;
  }
  while ( ip == ' ' || ip == '\t'){
    bop.input_index++; 
  }
  return bop;
  #undef ip
}

//id     <- [a-zA-Z_][a-zA-z0-9_.]*
Shuttle bcParseId (Shuttle &shuttle){
  #define ip bop.input_string[bop.input_index]
  Shuttle bop(shuttle, Rule::ID);
  if ( ip >= 'A' && ip <= 'Z' || ip >= 'a' & ip <= 'z' || ip == '_'){
    bop.input_index++;
  }
  else {
    bop.match = false;
    return bop;
  }
  while (true){
    if ( ip >= 'A' && ip <= 'Z' || ip >= 'a' & ip <= 'z' || ip == '_'
      || ip == '.' || ip >= '0' && ip <= '9'){
      bop.input_index++;
    }
    else {return bop;}

  }
  #undef ip
}

//colon   <- ':'
Shuttle bcParseColon (Shuttle &shuttle){
  #define ip bop.input_string[bop.input_index]
  Shuttle bop(shuttle, Rule::COLON);
  if ( ip == ':'){
    bop.input_index ++;
    return bop;
  }
  else {
    bop.match = false;
    return bop;
  }

  #undef ip
}

//label   <- colon id 
Shuttle bcParseLabel (Shuttle &shuttle){
  #define ip bop.input_string[bop.input_index]
  Shuttle bop (shuttle, Rule::LABEL);
  Shuttle false_bop = bop;
  false_bop.match = false;

  bop = ParseGrammar(shuttle, bcParseColon);
  if (bop.match != true){
    return false_bop;
  }
  bop = ParseGrammar(bop, bcParseId);
  if (bop.match != true){
    return false_bop;
  }
  return bop;
  #undef ip
} 