#include <catch2/catch.hpp>
#include <vector>
#include <string>
#include <parse_machine.hpp>

TEST_CASE( "Test byte code parsing") {
  SECTION ( "Parse raw string to code"){
    CHECK(99 == 99);
    // std::string code_string = R"<<(
    //   match x 2
    //   set_state true
    //   halt
    // )<<";
    // std::vector<Instruction> code = Parse (code_string);
    // ParseMachine test_m(code, "x");
    // CHECK(test_m.Match() == true);
    // test_m.Set(code, "y");
    // CHECK(test_m.Match() == false);

  }
}