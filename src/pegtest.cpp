#include <pegtest.hpp>
#include <iostream>

//returns contents of next matched '()' starting at index.
gobble::gobble (const std::string& parentString, int index){
    content = "";
    status = false;
    consumed = 0;
    gobble_error.type = ErrorType::NON_ERROR;
    gobble_error.errorMSG = "Unknown Error in gobble.";

    //First see if index is even inside string
    if (parentString.size() - 1 < index){
        gobble_error.type = ErrorType::OUT_OF_BOUNDS;
        gobble_error.errorMSG = "Attempt to read beyond string limits.";
        return;
    }
    
    char currentChar = parentString[index];
    //find next list
    int inc = ConsumeSpaces(parentString, index);
    index += inc;
    consumed += inc;
    if (parentString[index] != '(' ){
        gobble_error.errorMSG = "Error: Illegal characters found before list.";
        gobble_error.type = ErrorType::BAD_INPUT;
        std::cout << "Found bad input at: " << index << std::endl;
        std::cout << "And the input is: " << parentString[index-2] << std::endl;
        std::cout << "size is: " << parentString.size() << std::endl;
        return;
    }
    //If found nothing, default fail status remains.
    if (parentString.size() < index) return;

    //If we are here, we must be at a '(' and want to ignore this and push forward.
    consumed++;
    index++;

    auto depth {1}; //depth of nested parens
    while (parentString.size() >= index){
        //We don't want white space
        int inc;
        inc = ConsumeSpaces(parentString, index);
        index += inc;
        consumed += inc;
        currentChar = parentString[index];
        consumed++;
        if (currentChar == '(' ){
           depth++;
        }
        if (currentChar == ')' ){
           depth--;
        }

        if (currentChar == ')' && depth == 0){
            status = true;
            //index not needed any more.
            //ending paren is not part of content
            break;
        }
        content += parentString[index];
        index++;
    }
}

testBatch::testBatch(const std::string& inputTests){
    //Convert input grammar string into test tree.
    int index {0};
    int testNumber {0};
    valid = true; 

    while (inputTests.size() > index){
        gobble test(inputTests, index);
        if (test.status == true){
            grammarTest newTest(test.content);
            index += test.consumed;
            batch.push_back(newTest);
            testNumber += 1;
        }
        else {
            if (testNumber == 0){
                valid = false;
                if (test.gobble_error.type != ErrorType::NON_ERROR){
                    batch_error.errorMSG = test.gobble_error.errorMSG;
                    batch_error.type = test.gobble_error.type;
                }
                else{
                    batch_error.errorMSG = "Error: No test cases found.";
                    batch_error.type = ErrorType::EMPTY_LIST;
                }
            } 
            break;
        }
    }
    for (int i = 0; batch.size() > i; i++){
        std::cout << "Test " << i+1 << ":" << std::endl;
        std::cout << batch[i].content << std::endl;
    }

}

grammarTest::grammarTest(const std::string& parseString){
    int testNumber {0};
    valid = true;

    content = parseString;
    
}

//returns number of space characters consumed if any
int ConsumeSpaces (const std::string& parseString, int index){
    int consumed {0};
    while(parseString.size() >= index){
        //std::cout << "At " << index << " character is: " << parseString[index] << std::endl;
        switch (parseString[index])
        {
            case ' ':
            case '\n':
            case '\t':
            case '\v':
            case '\r':
            case '\f':
            case 0:
                index++;
                consumed++;
                break;
            default:
                return consumed;
            break;
        }
    }
    return consumed;
}