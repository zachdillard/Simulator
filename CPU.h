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
	void fetch(Memory &memory, PCB &curPCB){
	//No while test yet
	//Gets value of current PC
		int curPC=curPCB.pc;
	//Gets memory value of current PC in RAM
		string memValue = memory.getRAM(curPC);
	//Updates PC by 1
		curPCB.pc += 1;
		
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
		
	//Gives execute our OPCODE, the type of instruction, and the entire instruction for later use
		execute(instructionType, decodedValue);
	}

	//PRECONDITION: The input must be a binary number in a string value
	//POSTCONDITION: Outputs a 6-bit opcode
	string getOpCode(string decodedValue){
		return decodedValue.substr(2,7);
	}
		
	//PRECONDITION: The input must be a binary number in a string value
	//POSTCONDITION: Outputs 2 bits that represents the instruction type to use
	string getInstructionType(string decodedValue){
		return decodedValue.substr(0,2);
	}
		
//EXECUTE
    //Depending on the opcode, execute that particular method or statement
	//all methods called are to-be-implemented
	void execute(string instructionType, string statement){
		if(instructionType == "00")
			arithmeticFormat(statement);
		else if(instructionType == "01")
			conditionalFormat(statement);
		else if(instructionType == "10")
			jumpFormat(statement);
		else if(instructionType == "11")
			IOFormat(statement);
	}

public:
  //One of these registers could be the Program Counter
  //OR maybe create a variable called PC
  //The project specifications states that the first to registers are special purpose
  //For now let's have it has a public variable so the Short Term scheduler can access it easily
    int PC; //Address or array index of the instruction in memory
    std::string registers[16];
};


#endif //SIMULATOR_CPU_H
