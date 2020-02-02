#pragma once
#include <string>
#include <parse_machine.hpp>


//parsing data types

//The parser functions should emit segments of a flat
//representation of a parse tree. A flat representation is
//chosen during parsing because as various rules are tested,
//tree segments may possibly be created and destroyed many times.
//We want to avoid allocating and de-allocating memory for nodes as each 
//grammar rule is tested. A flat structure is a list expression in form
//which can be made into a parse tree in one pass once the parsing
//is completed.
//The flat tree representation can be seen as a string of special characters.
//These characters in turn can be seen as tree building instructions.
enum class TreeCharType{
  TREE_EOF,
  ADDRESS, //signifies datum holds address on input string bound to this node
  LENGTH, //signifies length of substring at ADDRESS
  OPCODE, //signifies opcode associated with this node
  TREE //signifies data is tree structure char '(' or ')'
};

constexpr int tree_node_size {5};

typedef union TreeDatum{
  unsigned char character;
  unsigned int address;
  unsigned int length;
  Opcode opcode;

} TreeDatum;

typedef struct TreeChar{
  TreeCharType type;
  TreeDatum datum;
} TreeChar;

enum class Rule{
  UNSET_RULE,
  NUMBER,
  SPACE,
  ID,
  LABEL
};


typedef struct Shuttle{
  std::vector<TreeChar> &tree;
  unsigned long int tree_index{0};
  const std::string &input_string;
  unsigned long int input_index{0};
  bool match;
  bool expectNode {true};
  Rule rule;
  Shuttle(const std::string &input, std::vector<TreeChar> &tree );
  Shuttle(const std::string &input);
  Shuttle(const Shuttle &input_shuttle, Rule new_rule);
} Shuttle;

//byte code parsing functions

//Helper functions for instruction generation
std::vector<Instruction> Parse (std::string input_string);
Instruction ParseInstruction (std::string input_string);
Opcode GetOpcodeFromName(std::string opcode_name);

//General Grammar Parser
Shuttle ParseGrammar (Shuttle &shuttle,
  Shuttle (*ParseRule)(Shuttle &shuttle));

Shuttle bcParseNumber (Shuttle &shuttle); 
Shuttle bcParseSpace (Shuttle &shuttle); 
Shuttle bcParseId (Shuttle &shuttle); 
Shuttle bcParseLabel (Shuttle &shuttle); 