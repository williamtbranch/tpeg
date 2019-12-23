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
    std::cout << "Beginning read from file " << argv[1] << std::endl;
    std::ifstream ifs(argv[1]);
    
    std::string testString;
    testString.assign ((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    //Create test batch object
    testBatch *tests = new testBatch(testString);

    delete tests;

    return 0;
}