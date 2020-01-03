#include <parse_machine.hpp>

Argument::Argument (char input) {
    c = input;
}

Argument::Argument (unsigned int input) {
    address = input;
}


Instruction::Instruction(Opcode input_opcode, Argument input_argument)
    :opcode(input_opcode), argument(input_argument) {

}

Instruction::Instruction(Opcode input_opcode, char input_argument)
    :opcode(input_opcode), argument(input_argument){

}

Instruction::Instruction(Opcode input_opcode, unsigned int input_argument)
    :opcode(input_opcode), argument(input_argument){

}

ParseMachine::ParseMachine(){
    input_string_.push_back(0);
}