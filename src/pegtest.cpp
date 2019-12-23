#include <pegtest.hpp>
#include <iostream>

//returns contents of next matched '()' starting at index.
gobble::gobble (std::string parentString, int index){
    content = "";
    status = false;
    consumed = 0;
    
    //find next list
    while (parentString.size() >= index && parentString[index] != '(' ){
        index++;
        consumed++;
    }
std::cout << "found first paren";
    //If found nothing, default fail status remains.
    if (parentString.size() < index) return;

    //If we are here, we must be at a '(' and want to ignore this and push forward.
    consumed++;
    index++;

    auto depth {1}; //depth of nested parens
    while (parentString.size() >= index){
        consumed++;
        if (parentString[index] == '(' ){
           depth++;
           std::cout << '(';
        }
        if (parentString[index] == ')' ){
           depth--;
           std::cout << ')';
        }

        if (parentString[index] == ')' && depth == 0){
            status = true;
            //index not needed any more.
            break;
        }
        content += parentString[index];
        index++;
    }
}

testBatch::testBatch(std::string inputTests){
    //Convert input grammar string into test tree.
    std::cout << "Beginning parse." << std::endl;
    int index {0};
    gobble test(inputTests, index);
    std::cout << test.content << std::endl;
    //grammarTest newTest(inputTests, index);

}

grammarTest::grammarTest(const std::string &parseString, int index){
    isValid = false;
    
}

