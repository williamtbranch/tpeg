#include <pegtest.hpp>
#include <iostream>

//returns contents of next matched '()' starting at index.
gobble::gobble (const std::string& parentString, int index){
    content = "";
    status = false;
    consumed = 0;
    gobble_error.type = ErrorType::NON_ERROR;
    gobble_error.errorMSG = "Expecting no error in gobble.";

    //First see if index is even inside string
    if (parentString.size()  < index){
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
        //Check if at EOF - this is not a parse error
        if (parentString[index] == 0){
            return;
        }
        //This is a parse error
        std::cout << "integer of character is: " << (int)parentString[index] << std::endl;
        std::cout << "character is: " << parentString[index] << std::endl;
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
    if (depth > 0){
        gobble_error.type = ErrorType::UNBALANCED;
        gobble_error.errorMSG = "Reached end of input before finding ')'";
    }
}

testBatch::testBatch(const std::string& inputTests){
    //Convert input grammar string into test tree.
    int index {0};
    int testNumber {0};
    valid = true; 
    std::string errorMsg;

    while (inputTests.size() > index){
        gobble test(inputTests, index);
        if (test.status == true){
            grammarTest newTest(test.content);
            if (newTest.grammarTestError.type != ErrorType::NON_ERROR){
                batch_error.type = newTest.grammarTestError.type;
                batch_error.errorMSG = newTest.grammarTestError.errorMSG;
                batch_error.errorMSG += "\nError found in grammar test #";
                batch_error.errorMSG += std::to_string(batch.size() + 1);
                valid = false;
                return;
            }
            index += test.consumed;
            batch.push_back(newTest);
            testNumber += 1;
        }
        else {
            if (test.gobble_error.type != ErrorType::NON_ERROR){
                valid = false;
                batch_error.errorMSG = test.gobble_error.errorMSG;
                batch_error.type = test.gobble_error.type;
                batch_error.errorMSG += "\nError found in grammar test #";
                batch_error.errorMSG += std::to_string(batch.size() + 1);
                return;
            }
            
            if (testNumber == 0){
                valid = false;
                batch_error.errorMSG = "Error: No test cases found.";
                batch_error.type = ErrorType::EMPTY_LIST;
                return;
            } 
            break;
        }
    }
}

grammarTest::grammarTest(const std::string& parseString){
    int testNumber {0};
    valid = true;
    content = parseString;
    int index {0};
    //Get to first non-space
    index += ConsumeSpaces(content, index);
    //Grab Name from first field
    while(content.size() > index){
        //ascii chars from 0 to z including several symbols
        if (content[index] > 47 && content[index] < 123){
            testName.push_back(content[index]);
            index++;
        }
        else break;
    }

    //If no name return with error flagged
    if (testName.size() == 0){
        valid = false;
        grammarTestError.type = ErrorType::BAD_INPUT;
        grammarTestError.errorMSG = "Grammar Test missing Grammar Test Name.";
        return;
    }

    //get grammardef
    gobble test(parseString, index);
    grammarDef newGrammar(test.content);
    grammar = newGrammar;
    index += test.consumed;

    //get test cases
    while(parseString.size() > index){
        test = gobble(parseString, index);
        tests.push_back(testCase(test.content));
        index += test.consumed;
    }


}

testCase::testCase (const std::string& parseString){
    int index {0};
    //get test type
    gobble chunk(parseString, index);
    index += chunk.consumed;
    if (chunk.content == "match"){
        type = testType::MATCH;
    }
    else {
        type = testType::SEARCH;
    }

    //get expression
    chunk = gobble(parseString, index);
    index += chunk.consumed;
    input = chunk.content;

    //get expected value

    chunk = gobble(parseString, index);
    expected = chunk.content;
}


grammarDef::grammarDef(const std::string& parseString){
    int index {0};
    while (parseString.size() > index){
        gobble expressionString(parseString, index);
        pegExpression expression(expressionString.content);
        index += expressionString.consumed;
        expressions.push_back(expression);
    }
}

grammarDef::grammarDef(){
}

pegExpression::pegExpression(const std::string& parseString){
    int index {0};
    gobble ruleName(parseString, 0);
    name = ruleName.content;
    index += ruleName.consumed;
    rule = gobble(parseString, index).content;
}

//returns number of space characters consumed if any
int ConsumeSpaces (const std::string& parseString, int index){
    int consumed {0};
    while(parseString.size() > index){
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