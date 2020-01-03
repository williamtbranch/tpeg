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

TEST_CASE( "Test ExpressionMachine behavior", "[ParseMachine]" ) {
    std::string input("a");
    std::vector<Instruction> code;
    //code.push_back(Instruction (Opcode::MATCH, 'a'));

    //ParseMachine test_machine(input, code);
    //REQUIRE( test_machine.input_string_[0] == 0); 

}
