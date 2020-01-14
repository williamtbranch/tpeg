#include <catch2/catch.hpp>
#include <iostream>
#include <tpeg.hpp>
#include <bc_parse.hpp>
#include <parse_machine.hpp>
#include <string>
#include <vector>

TEST_CASE( "ParseMachine", "[ParseMachine]" ) {
  //Parse machine input string should always have terminating zero
  //Initializes thus
  SECTION ( "Initializeemptyparsemachine" ) {
    ParseMachine test_m;
    CHECK (test_m.GetParseString() == "");
    CHECK (test_m.GetParseCode().size() == 0);
  }

  //OUT_OF_BOUNDS instruction should be internal only
  //if user supplies one or more at end of code, they are removed
  //and replaced with single HALT Since they are semantically identical
  SECTION ( "Endingoutofbounds"){
    std::vector<Instruction> code;
    code.push_back(Instruction(Opcode::MATCH, 'a', 1));
    code.push_back(Instruction(Opcode::MATCH, 'a', 1));
    code.push_back(Instruction(Opcode::MATCH, 'b', 1));
    code.push_back(Instruction(Opcode::MATCH, 'a', 1));
    code.push_back(Instruction(Opcode::JUMP, 1));
    code.push_back(Instruction(Opcode::OUT_OF_BOUNDS));
    code.push_back(Instruction(Opcode::OUT_OF_BOUNDS));
    code.push_back(Instruction(Opcode::OUT_OF_BOUNDS));
    ParseMachine test_m(code, "aaba");
    CHECK (test_m.GetParseCode().size() == 6);
    CHECK (test_m.GetParseCode()[5].opcode == Opcode::HALT);
  }

  SECTION ( "Manually setting") {
    ParseMachine test_m;
    test_m.SetParseString("abba");

    //check parse string is size 4
    CHECK (test_m.GetParseString().size() == 4);

    //check parse string is as expected: no internal mangling
    CHECK (test_m.GetParseString() == "abba");

    //setting code up
    std::vector<Instruction> code;
    code.push_back(Instruction(Opcode::MATCH, 'a', 2));
    code.push_back(Instruction(Opcode::MATCH, 'a', 2));
    code.push_back(Instruction(Opcode::MATCH, 'b', 2));
    code.push_back(Instruction(Opcode::JUMP, 1));

    test_m.SetParseCode(code);

    //check instruction size is correct
    CHECK (test_m.GetParseCode().size() == 4);

    //check arbitrary instruction in code: no internal mangling
    CHECK (test_m.GetParseCode()[3].opcode == Opcode::JUMP);
    CHECK (test_m.GetParseCode()[3].argument1.address == 1);
  }

  SECTION ( "Resettingparsestring"){
    //setting code up
    std::vector<Instruction> code;
    code.push_back(Instruction(Opcode::MATCH, 'a', 2));
    code.push_back(Instruction(Opcode::MATCH, 'a', 2));
    code.push_back(Instruction(Opcode::MATCH, 'b', 2));
    code.push_back(Instruction(Opcode::MATCH, 'a', 2));
    code.push_back(Instruction(Opcode::JUMP, 5));
    code.push_back(Instruction(Opcode::MATCH, 'a', 2));
    code.push_back(Instruction(Opcode::MATCH, 'b', 2));
    code.push_back(Instruction(Opcode::JUMP, 1));
    ParseMachine test_m(code, "abbbbbbbba");

    std::vector<Instruction> code2;
    code2.push_back(Instruction(Opcode::MATCH, 'x', 2));
    code2.push_back(Instruction(Opcode::MATCH, 'y', 2));
    code2.push_back(Instruction(Opcode::MATCH, 'y', 2));
    code2.push_back(Instruction(Opcode::MATCH, 'x', 2));
    code2.push_back(Instruction(Opcode::JUMP, 2));

    test_m.SetParseCode(code2);
    test_m.SetParseString("xyyx");

    //check parse string is size 4
    CHECK (test_m.GetParseString().size() == 4);

    //check parse string is as expected: no internal mangling
    CHECK (test_m.GetParseString() == "xyyx");

    //check instruction size is correct
    CHECK (test_m.GetParseCode().size() == 5);

    //check arbitrary instruction in code: no internal mangling
    CHECK (test_m.GetParseCode()[3].opcode == Opcode::MATCH);
    CHECK (test_m.GetParseCode()[3].argument1.c == 'x');
  }

}
