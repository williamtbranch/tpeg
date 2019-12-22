#include <pegtest.hpp>
#include <iostream>


testBatch::testBatch(std::string inputTests){
    //Convert input grammar string into test tree.
    std::cout << "Beginning parse." << std::endl;
    int index {0};
    grammarTest newTest(inputTests, index);

}

grammarTest::grammarTest(const std::string &parseString, int index){
    isValid = false;
    
}

