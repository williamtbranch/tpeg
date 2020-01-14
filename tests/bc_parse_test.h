#include <catch2/catch.hpp>
#include <vector>
#include <string>
#include <parse_machine.hpp>
#include <bc_parse.hpp>

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