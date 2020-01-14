#include <catch2/catch.hpp>
#include <iostream>
#include <tpeg.hpp>
#include <bc_parse.hpp>
#include <parse_machine.hpp>
#include <string>
#include <vector>

TEST_CASE ( "Basic PEG patterns" ){
  SECTION ( "Simple Matching" ){
    //single character
    std::vector<Instruction> code {Parse(R"(
      match a 2
      set_valid true
      halt
    )")};
    ParseMachine test_m(code, "a");
    CHECK(test_m.Match() == true);
    test_m.Set(code, "b");
    CHECK(test_m.Match() == false);

    //multi-character
    code.clear();
    code = Parse(R"(
     match a 4
     match b 4
     match c 4
     set_valid true
     halt 
    )");
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
    std::vector<Instruction> code {Parse(R"(
      match a 6
      match b 6
      match b 4
      jump 2
      match a 6
      set_valid true
      halt
    )")};
    ParseMachine test_m(code, "abbbbbbbba");
    CHECK(test_m.Match() == true);
    test_m.SetParseString("aba");
    CHECK(test_m.Match() == true);
    test_m.SetParseString("abbbb");
    CHECK(test_m.Match() == false);
    test_m.SetParseString("");
    CHECK(test_m.Match() == false);


  }
}