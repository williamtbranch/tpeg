#include <catch2/catch.hpp>
#include <iostream>
#include <tpeg.hpp>
#include <expression_machine.hpp>
#include <string>
#include <vector>


TEST_CASE( "Test ExpressionMachine behavior", "[ExpressMachine]" ) {
    std::string input("a");
    std::vector<Instruction> code {MATCH};

    ExpressionMachine test_machine(input, code);
    REQUIRE( test_machine.input_string_[0] == 0); 

}
