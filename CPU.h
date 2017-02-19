#include <string>
#include "Memory.h"
#include <bitset>
#include <sstream>
#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H

using namespace std;


class CPU {
    //while there are instructions to fetch
    //FETCH
    //Go to the PC and get its value, this is the address of the instruction
    //Go to that address and retrieve the value
    //Give this value to DECODE
    //Update PC by 1
    void fetch(PCB* curPCB){
        //No while test yet
        //Gets value of current PC
        PC =curPCB->pc;
        //Gets memory value of current PC in RAM
        string memValue = mem->getRAM(PC);
        //Updates PC by 1
        PC += 1;
        
        //Sends instruction to decoder
        decoder(memValue);
    }
    //DECODE
    //Read each bit to figure out if I/O instruction or Compute
    //Figure out the opcode
    //Give EXECUTE something to let it know which opcode/method to execute
    void decoder(string memValue){
        //Converts hex input to binary
        stringstream ss;
        ss << hex << memValue;
        unsigned placeHolder;
        ss >> placeHolder;
        bitset<32> ourValue(placeHolder);
        string decodedValue = ourValue.to_string();
        
        //Gets our OPCODE and instruction format
        string instructionType = getInstructionType(decodedValue);
        string opcode = getOpCode(decodedValue);
        //Gives execute our OPCODE, the type of instruction, and the entire instruction for later use
        execute(instructionType, opcode, decodedValue);
    }
    
    //PRECONDITION: The input must be a binary number in a string value
    //POSTCONDITION: Outputs a 6-bit opcode
    string getOpCode(string decodedValue){
        return decodedValue.substr(2,8);
    }
    //PRECONDITION: The input must be a binary number in a string value
    //POSTCONDITION: Outputs 2 bits that represents the instruction type to use
    string getInstructionType(string decodedValue){
        return decodedValue.substr(0,2);
    }

    //EXECUTE
    //Depending on the opcode, execute that particular method or statement
    //all methods called are to-be-implemented
    void execute(string instructionType, string opcode, string statement){
        int msb = stoul(instructionType, nullptr, 2);
        int oc = stoul(opcode, nullptr, 2);
        switch(msb)
        {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                IOformat(statement)
                break;
        }
    }
public:
    //One of these registers could be the Program Counter
    //OR maybe create a variable called PC
    //The project specifications states that the first to registers are special purpose
    //For now let's have it has a public variable so the Short Term scheduler can access it easily
    CPU(Memory* memory)
    {
        mem = memory;
        registers[1] = "0x0";
    }
    void IOformat(string statement)
    {
        int reg1;
        int reg2;
        int address;
        int opcode;
        
        opcode = stoul(getOpCode(statement), nullptr, 2);
        reg1 = stoul(statement.substr(8, 12), nullptr, 2);
        reg2 = stoul(statement.substr(12, 16), nullptr, 2);
        address = stoul(statement.substr(16, 32), nullptr, 2);
        
        
        switch(opcode)
        {
            case 0:
                RD(reg1, reg2, address);
            case 1:
                WR(reg1, reg2, address);
        }
    }
    void UnconditionalJump(string statement)
    {
        int addresss;
    }
    void ConditionalBranch(string statement)
    {
        int bReg;
        int dReg;
        int address;
    }
    void ArithmeticFormat(string statement)
    {
        int sReg1;
        int sReg2;
        int dReg;
    }
    
    void RD(int reg1, int reg2, int address)
    {
        if(reg2 == 0)
            registers[reg1] = mem->getRAM(address);
        else
            registers[reg1] = registers[reg2];
    }
    void WR(){};
    int PC; //Address or array index of the instruction in memory
    std::string registers[16];
private:
    Memory* mem;
};


#endif //SIMULATOR_CPU_H