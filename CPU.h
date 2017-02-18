#include <string>
#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H


class CPU {
//while there are instructions to fetch
  //FETCH
    //Go to the PC and get its value, this is the address of the instruction
    //Go to that address and retrieve the value
    //Give this value to DECODE
    //Update PC by 1
  //DECODE
    //Read each bit to figure out if I/O instruction or Compute
    //Figure out the opcode
    //Give EXECUTE something to let it know which opcode/method to execute
  //EXECUTE
    //Depending on the opcode, execute that particular method or statement
  
public:
  //One of these registers could be the Program Counter
  //OR maybe create a variable called PC
  //The project specifications states that the first to registers are special purpose
  //For now let's have it has a public variable so the Short Term scheduler can access it easily
    int PC; //Address or array index of the instruction in memory
    std::string registers[16];
};


#endif //SIMULATOR_CPU_H
