#include <catch2/catch.hpp>
#include <iostream>
#include <tpeg.hpp>


TEST_CASE( "Test basic functionality", "[single-file]" ) {
    REQUIRE( printHello() == 1 );
}
