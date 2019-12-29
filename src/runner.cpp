#include <iostream>
#include <fstream>
#include <pegtest.hpp>

//Runner to test the tpeg parser
int main (int argc, char *argv[]){
    if (argc != 2){
        std::cout << "Runner requires a single input file argument." << std::endl;
        return 1;
    }

    //Get file into test string
    std::ifstream ifs(argv[1]);
    if (ifs.fail()){
        std::cout << "Could not open file " << argv[1] << std::endl;
    }
    
    std::string testString;
    testString.assign ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    //Create test batch object
    testBatch *tests = new testBatch(testString);

    if (tests->valid == false) {
        std::cout << tests->batch_error.errorMSG << std::endl;
    }

    //walk tree and print content
    for (int i = 0; tests->batch.size() > i; i++ ){

        std::cout << tests->batch[i].testName << std::endl;
        for (int x = 0; tests->batch[i].grammar.expressions.size() > x; x++){
            std::cout << "  " << tests->batch[i].grammar.expressions[x].name
            << " <- "
            << tests->batch[i].grammar.expressions[x].rule << std::endl;

        }
        std::cout << std::endl;

        for (int x = 0; tests->batch[i].tests.size() > x; x++){
            if (tests->batch[i].tests[x].type  == testType::MATCH) {
                std::cout << "  " << "Match" << std::endl;
            }
            else{
                std::cout << "  " << "Search" << std::endl;
            }
            std::cout << "  " << tests->batch[i].tests[x].input << std::endl;
            std::cout << "  " << tests->batch[i].tests[x].expected << std::endl << std::endl;
        }
    }

    delete tests;

    return 0;
}