#include <catch2/catch.hpp>
#include <iostream>
#include <tpeg.hpp>
#include <parse_machine.hpp>
#include <string>
#include <vector>


TEST_CASE( "Test Instruction Constructor", "[Instruction]"){
  //default constructor for char Argument
  Argument argument = 'a';
  Instruction test_instruction(Opcode::MATCH, argument);
  CHECK ( test_instruction.argument.c == 'a');

  //takes opcode correctly
  CHECK ( test_instruction.opcode == Opcode::MATCH);

  //default constructor for unsigned int Argument
  Argument argument2 = (unsigned int)65535;
  Instruction test_instruction2(Opcode::JUMP, argument2);
  CHECK ( test_instruction2.argument.address == 65535);

  //test directly setting instruction arguments
  Instruction test_instruction3(Opcode::MATCH, 'b');
  CHECK ( test_instruction3.argument.c == 'b');
  Instruction test_instruction4(Opcode::JUMP, (unsigned int)4545);
  CHECK ( test_instruction4.argument.address == 4545);

}

TEST_CASE( "Test ParseMachine behavior", "[ParseMachine]" ) {
  //Parse machine input string should always have terminating zero
  //Initializes thus
  SECTION ( "Initialize empty parse machine" ) {
    ParseMachine test_m;
    CHECK (test_m.GetParseString() == "");
    CHECK (test_m.GetParseCode().size() == 0);
  }



  SECTION ( "Initialize2 populated parse machine" ) {
    std::vector<Instruction> code;
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    code.push_back(Instruction(Opcode::MATCH, 'b'));
    code.push_back(Instruction(Opcode::JUMP, (unsigned int)1));

    ParseMachine test_m(code, "aaba");

    //check parse string is size 4
    CHECK (test_m.GetParseString().size() == 4);

    //check parse string is as expected: no internal mangling
    CHECK (test_m.GetParseString() == "aaba");

    //check instruction size is correct
    CHECK (test_m.GetParseCode().size() == 4);

    //check arbitrary instruction in code: no internal mangling
    CHECK (test_m.GetParseCode()[3].opcode == Opcode::JUMP);
    CHECK (test_m.GetParseCode()[3].argument.address == 1);
  }

  //OUT_OF_BOUNDS instruction should be internal only
  //if user supplies one or more at end of code, they are removed
  //and replaced with single HALT Since they are semantically identical
  SECTION ( "Ending 'out of bounds' instruction removed"){
    std::vector<Instruction> code;
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    code.push_back(Instruction(Opcode::MATCH, 'b'));
    code.push_back(Instruction(Opcode::MATCH, 'b'));
    code.push_back(Instruction(Opcode::JUMP, (unsigned int)1));
    code.push_back(Instruction(Opcode::OUT_OF_BOUNDS));
    code.push_back(Instruction(Opcode::OUT_OF_BOUNDS));
    code.push_back(Instruction(Opcode::OUT_OF_BOUNDS));
    ParseMachine test_m(code, "aaba");
    CHECK (test_m.GetParseCode().size() == 6);
    CHECK (test_m.GetParseCode()[5].opcode == Opcode::HALT);
  }

  SECTION ( "Manually setting parse string and code") {
    ParseMachine test_m;
    test_m.SetParseString("abba");

    //check parse string is size 4
    CHECK (test_m.GetParseString().size() == 4);

    //check parse string is as expected: no internal mangling
    CHECK (test_m.GetParseString() == "abba");

    //setting code up
    std::vector<Instruction> code;
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    code.push_back(Instruction(Opcode::MATCH, 'b'));
    code.push_back(Instruction(Opcode::JUMP, (unsigned int)1));

    test_m.SetParseCode(code);

    //check instruction size is correct
    CHECK (test_m.GetParseCode().size() == 4);

    //check arbitrary instruction in code: no internal mangling
    CHECK (test_m.GetParseCode()[3].opcode == Opcode::JUMP);
    CHECK (test_m.GetParseCode()[3].argument.address == 1);
  }

  SECTION ( "Resetting parse string and code should clear old values"){
    //setting code up
    std::vector<Instruction> code;
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    code.push_back(Instruction(Opcode::MATCH, 'b'));
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    code.push_back(Instruction(Opcode::JUMP, (unsigned int)5));
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    code.push_back(Instruction(Opcode::MATCH, 'b'));
    code.push_back(Instruction(Opcode::JUMP, (unsigned int)1));
    ParseMachine test_m(code, "abbbbbbbba");

    std::vector<Instruction> code2;
    code2.push_back(Instruction(Opcode::MATCH, 'x'));
    code2.push_back(Instruction(Opcode::MATCH, 'y'));
    code2.push_back(Instruction(Opcode::MATCH, 'y'));
    code2.push_back(Instruction(Opcode::MATCH, 'x'));
    code2.push_back(Instruction(Opcode::JUMP, (unsigned int)2));

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
    CHECK (test_m.GetParseCode()[3].argument.c == 'x');
  }

  SECTION ( "Simple Matching" ){
    //single character
    std::vector<Instruction> code;
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    ParseMachine test_m(code, "a");
    CHECK(test_m.Match() == true);

    test_m.Set(code, "b");
    CHECK(test_m.Match() == false);

    //multi-character
    code.clear();
    code.push_back(Instruction(Opcode::MATCH, 'a'));
    code.push_back(Instruction(Opcode::MATCH, 'b'));
    code.push_back(Instruction(Opcode::MATCH, 'c'));
    test_m.Set(code, "abc");
    CHECK(test_m.Match() == true);
    test_m.Set(code, "cba");
    CHECK(test_m.Match() == false);

    //handles running out of string to process
    test_m.Set(code, "a");
    CHECK(test_m.Match() == false);
    test_m.Set(code, "b");
    CHECK(test_m.Match() == false);

    //does not incorrectly match when last characters match
    test_m.Set(code, "xxc");
    CHECK(test_m.Match() == false);
  }

  SECTION ( "Repeat one or more times '+' matching"){

  }

}
