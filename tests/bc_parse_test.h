#include <catch2/catch.hpp>
#include <vector>
#include <string>
#include <parse_machine.hpp>
#include <bc_parse.hpp>
#include <iostream>

TEST_CASE( "Single instruction correctness") {
  SECTION ( "MATCH parse"){
    Instruction instruction {ParseInstruction("match a 5")};
    CHECK (instruction.opcode == Opcode::MATCH);
    CHECK (instruction.argument1.c == 'a');
    CHECK (instruction.argument2.address == 5);

    //bad input
    Instruction badi1 {ParseInstruction("match a")};
    CHECK (badi1.opcode == Opcode::UNSET);
    Instruction badi2 {ParseInstruction("match")};
    CHECK (badi2.opcode == Opcode::UNSET);
  }
  SECTION ( "SET_VALID parse" ){
    Instruction instruction {ParseInstruction("set_valid true")};
    CHECK (instruction.opcode == Opcode::SET_VALID);
    CHECK (instruction.argument1.flag == true);
    Instruction instruction2 {ParseInstruction("set_valid false")};
    CHECK (instruction2.argument1.flag == false);

    //bad input
    Instruction badi1 {ParseInstruction("set_valid")};
    CHECK (badi1.opcode == Opcode::UNSET);
  }
  SECTION ( "HALT parse" ){
    Instruction instruction {ParseInstruction("halt")};
    CHECK (instruction.opcode == Opcode::HALT);
  }
}

TEST_CASE( "Parsing multi-line correctness" ){
  SECTION ("Basic"){
    std::vector<Instruction> code {Parse(R"(
      match a 4
      match b 4
      match c 4
      set_valid true
      halt
    )")};
    CHECK(code[0].opcode == Opcode::MATCH);
    CHECK(code[3].opcode == Opcode::SET_VALID);
    CHECK(code[2].argument1.c == 'c');
  }
}

//space       <- [\s\t]+
//number      <- [1-9][0-9]+
//id          <- [a-zA-Z_][a-zA-z0-9_.]*
//colon       <- ':'
//label       <- colon id 


//space  <- [\s\t]+
TEST_CASE( "bcParseSpace test"){
  SECTION ("Basic Not match 'aa'"){
    std::vector<TreeChar> tree;

    std::string input {"aa"};

    input.push_back(0);
    Shuttle bop {input, tree};
    Shuttle bop_out = bcParseSpace(bop);

    CHECK(bop_out.match == false);
    CHECK(bop_out.input_index == 0);
  }
  SECTION ("Basic Match space ' '"){
    std::string input {" "};

    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseSpace(bop);
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 1);
  }
  SECTION ("Basic Match tab"){
    std::string input {"\t"};

    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseSpace(bop);
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 1);
  }
  SECTION ("Basic Match multiple type of spaces 'tab,space'"){
    std::string input {"\t "};

    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseSpace(bop);
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 2);
  }
  SECTION ("Basic Match multiple space with extra stuff"){
    std::string input {"\t  \t stuff"};

    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseSpace(bop);
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 5);
  }
}

//number <- [1-9][0-9]+
TEST_CASE( "bcParseNumber test"){
  SECTION ("Basic Not match 'aa'"){
    std::string input {"aa"};
    
    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseNumber(bop);
    CHECK(bop_out.match == false);
    CHECK(bop_out.input_index == 0);
  }
  SECTION ("Basic Match digit 1"){
    std::string input {"1"};

    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseNumber(bop);
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 1);
  }
  SECTION ("Not match digits starting with 0"){
    std::string input {"09"};

    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseNumber(bop);
    CHECK(bop_out.match == false);
    CHECK(bop_out.input_index == 0);
  }
  SECTION ("Match multiple digits"){
    std::string input {"208734 stuff"};

    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseNumber(bop);
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 6);
  }
}

//id     <- [a-zA-Z_][a-zA-z0-9_.]*
TEST_CASE( "bcParseId test"){
  SECTION ("Basic Not match '55'"){
    std::string input {"55"};
    
    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseId(bop);
    CHECK(bop_out.match == false);
    CHECK(bop_out.input_index == 0);
    CHECK(bop_out.rule == Rule::ID);
  }
  SECTION ("Basic match 'a stuff'"){
    std::string input {"a stuff"};
    
    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseId(bop);
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 1);
  }
  SECTION ("Match 'Z_b8.5a+ stuff'"){
    std::string input {"Z_b8.5a+ stuff"};
    
    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseId(bop);
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 7);
  }
  SECTION ("Match '_aZ_b8.095a+ stuff'"){
    std::string input {"_aZ_b8.095a+ stuff"};
    
    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseId(bop);
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 11);
  }
}

//General parser testing starts here because at least a few parsing primitives
//must already be functional as this function depends on them.
//Additionally a large set of parsing rules depends on ParseGrammar so it
//does not make sense to test this any later.
TEST_CASE( "ParseGrammar test"){

  SECTION ("Basic Not match '55'"){
    std::string input {"55"};
    
    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out {ParseGrammar(bop, bcParseNumber)};
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 2);
    CHECK(bop_out.rule == Rule::NUMBER);
  }

  SECTION ("Basic tree match"){
    std::string input {"5655"};
    
    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out {ParseGrammar(bop, bcParseNumber)};


    CHECK(bop_out.tree[0].type == TreeCharType::TREE);
    CHECK(bop_out.tree[1].type == TreeCharType::RULE);
    CHECK(bop_out.tree[2].type == TreeCharType::ADDRESS);
    CHECK(bop_out.tree[3].type == TreeCharType::LENGTH);
    CHECK(bop_out.tree[4].type == TreeCharType::TREE);
    CHECK(bop_out.tree[0].datum.character == '(');
    CHECK(bop_out.tree[1].datum.rule == Rule::NUMBER);
    CHECK(bop_out.tree[2].datum.address == 0);
    CHECK(bop_out.tree[3].datum.length == 4);
    CHECK(bop_out.tree[4].datum.character == ')');
    CHECK(bop_out.tree_index == 5);
  }
}

//colon   <- ':' 
TEST_CASE( "bcParseColon test"){
  SECTION ("Basic Not match '55'"){
    std::string input {"55"};
    
    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseColon(bop);
    CHECK(bop_out.rule == Rule::COLON);
    CHECK(bop_out.match == false);
    CHECK(bop_out.input_index == 0);
  }
  SECTION ("Basic Not match '55'"){
    std::string input {":5kj"};
    
    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = bcParseColon(bop);
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 1);
  }
}

//label   <- colon id 
TEST_CASE( "bcParseLabel test"){
  SECTION ("Basic Not match '55'"){
    std::string input {"55"};
    
    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = ParseGrammar(bop, bcParseLabel);
    CHECK(bop_out.rule == Rule::LABEL);
    CHECK(bop_out.match == false);
    CHECK(bop_out.input_index == 0);
  }
  SECTION ("Basic match ':hello stuff'"){
    std::string input {":hello stuff"};
    
    std::vector<TreeChar> tree;
    input.push_back(0);
    Shuttle bop {input, tree};

    Shuttle bop_out = ParseGrammar(bop, bcParseLabel);
    CHECK(bop_out.match == true);
    CHECK(bop_out.input_index == 6);
    CHECK(bop_out.tree[0].type == TreeCharType::TREE);
    CHECK(bop_out.tree[1].type == TreeCharType::RULE);
    CHECK(bop_out.tree[2].type == TreeCharType::ADDRESS);
    CHECK(bop_out.tree[3].type == TreeCharType::LENGTH);
    CHECK(bop_out.tree[4].type == TreeCharType::TREE);
    CHECK(bop_out.tree[0].datum.character == '(');
    CHECK(bop_out.tree[1].datum.rule == Rule::LABEL);
    CHECK(bop_out.tree[2].datum.address == 0);
    CHECK(bop_out.tree[3].datum.length == 6);
    CHECK(bop_out.tree[4].datum.character == '(');
    PrintTree(bop_out);
    // CHECK(bop_out.tree_index == 4);
  }
}