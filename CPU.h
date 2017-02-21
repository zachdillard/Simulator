#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H
#include <string>
#include "Memory.h"
#include <bitset>
#include <sstream>


using namespace std;


class CPU {
    //while there are instructions to fetch
    //FETCH
    //Go to the PC and get its value, this is the address of the instruction
    //Go to that address and retrieve the value
    //Give this value to DECODE
    //Update PC by 1
    void fetch(){
        //Gets memory value of current PC in RAM
        string memValue = mem->getRAM(PC);
        //Updates PC by 1
        ++PC;
	return memValue;
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
			case 2:
				ST(reg1, address);
			case 3: 
				LW(reg1, address);
        }
    }
    void UnconditionalJump(string statement)
    {
        int address;
		
		
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
	
    //Read
	//Reads the content of I/P buffer into an accumulator
    void RD(int reg1, int reg2, int address)
    {
        if(reg2 == 0)
            registers[reg1] = mem->getRAM(address);
        else
            registers[reg1] = registers[reg2];
    }
	//write 
	//Writes the content of accumulator into O/P buffer
	//Don't think this is right. Same as RD?
    void WR(int reg1, int reg2, int address)
	{
		if(reg2 == 0)
            registers[reg1] = mem->getRAM(address);
        else
            registers[reg1] = registers[reg2];
	}
	
	//store
	//stores the content of a register into an address
	void ST(int reg1, int address)
	{
		mem->setRam(address, registers[reg1]);
	}
	
	//load
	//loads the content of an address into a register
	void LW(int reg1, int address) 
	{
		registers[reg1] = mem->getRam(address);
	}
	
	//move
	//transfers the content of one register into another
	void MOV(int reg1, int reg2) 
	{
		registers[reg1] = registers[reg2];
	}
	
	//add
	//Adds the content of two S-regs into a D-reg
	void ADD(int sreg1, int sreg2, int dreg) 
	{
		registers[dreg] = registers[sreg1] + registers[sreg2];
	}
	
	//subtract
	// Subtracts content of two S-regs into D-reg
	void SUB(int sreg1, int sreg2, int dreg) 
	{
		registers[dreg] = registers[sreg1] - registers[sreg2];
	}
	
	//multiply
	// Mutiplies content of two S-regs into D-reg
	void MUL(int sreg1, int sreg2, int dreg) 
	{
		registers[dreg] = registers[sreg1] * registers[sreg2];
	}
	
	//divide
	//Divides content of two S-regs into D-reg
	void DIV(int sreg1, int sreg2, int dreg) 
	{
		registers[dreg] = (registers[sreg1] / registers[sreg2]);
	}
	
	//and gate
	//Logical AND of two S-regs into D-reg
	void AND(int sreg1, int sreg2, int dreg) 
	{
		if(registers[sreg1] == registers[sreg2])
			registers[dreg] = 1;
		else
			registers[dreg] = 0;
	}
	
	//or gate
	//Logical OR of two S-regs into D-reg
	void OR(int sreg1, int sreg2, int dreg) 
	{
		if(registers[sreg1] == 0 && registers[sreg2] == 0)
			registers[dreg] = 0;
		else
			registers[dreg] = 1;
	}
	
	//MOVI
	//Transfers addresses directly into a register
	void MOVI(int breg, int dreg, int address) 
	{
		if(dreg == 0) 
			mem->setRam(EffAddress(breg, address), registers[dreg]);
		else
			registers[breg] = registers[dreg];
	}
	
	//ADDI
	//Adds a data value directly to the content of a register
	void ADDI(int breg, int dreg, int address)
	{
		if(dreg == 0)
			mem->setRam(EffAddress(breg, address), (registers[dreg] += registers[breg]));
		else
			registers[dreg] += registers[breg];
	}
	
	//MULI
	//Multiplies a data value directly with the content of a register
	void MULI(int breg, int dreg, int address)
	{
		if(dreg == 0)
			mem->setRam(EffAddress(breg, address), (registers[dreg] *= registers[breg]));
		else
			registers[dreg] *= registers[breg];
	}
	
	//DIVI
	//Divides a data value directly to the content of a register
	void DIVI(int breg, int dreg, int address)
	{
		if(dreg == 0)
			mem->setRam(EffAddress(breg, address), (registers[dreg] /= registers[breg]));
		else
			registers[dreg] /= registers[breg];
	}
	
	//LDI
	//Loads a data/address directly to the content of a register
	void LDI(int breg, int dreg, int address) 
	{
		if(dreg == 0) 
			mem->setRam(EffAddress(breg, address), registers[dreg]);
		else
			registers[dreg] = registers[breg];
	}
	
	//SLT
	//Sets the D-reg to 1 if first S-reg is less than the B-reg; 0 otherwise
	void SLT(int sreg, int dreg, int breg) 
	{
		if(sreg < breg)
			registers[dreg] = 1;
		else
			registers[dreg] = 0;
	}
	
	//SLTI
	//Sets the D-reg to 1 if first S-reg is less than a piece of data; 0 otherwise
	void SLTI(int sreg, int dreg, int value, int address) 
	{
		if(sreg < value)
			registers[dreg] = 1;
		else
			registers[dreg] = 0;
	}
	
	//HLT
	//Logical end of Program
	void HLT() 
	{
		
	}
	
	//NOP
	//Does nothing and moves to next instruction
	void NOP() 
	{
		;
	}
	
	//JMP
	//Jumps to a specified locaiton
	void JMP(int address)
	{
		PC = mem->getRam(address);
	}
	
	//BEQ
	//Branches to an address when content of B-reg = D-reg
	void BEQ(int breg, int dreg, int address) 
	{
		if(breg == dreg) 
		{
			if(dreg == 0) 
				PC = EffAddress(breg, address);
			else 
				PC = address;
		}
		else
			;
	}
	
	//BNE
	//Branches to an address when content of B-reg != D-reg
	void BNE(int breg, int dreg, int address) 
	{
		if(breg != dreg) 
		{
			if(dreg == 0) 
				PC = EffAddress(breg, address);
			else 
				PC = address;
		}
		else 
			;
	}
	
	//BEZ
	//Branches to an address when content of B-reg = 0
	void BEZ(int breg, int dreg, int address) 
	{
		if(breg == 0) 
		{
			if(dreg == 0) 
				PC = EffAddress(breg, address);
			else 
				PC = address;
		}
		else 
			;
	}
	
	//BNZ
	//Branches to an address whne content of B-reg != 0
	void BNZ(int breg, int dreg, int address) 
	{
		if(breg != 0)
		{
			if(dreg == 0) 
				PC = EffAddress(breg, address);
			else 
				PC = address;
		}
		else
			;
	}
	
	//BGZ
	//Branches to an address when content of B-reg > 0
	void BGZ(int breg, int dreg, int address) 
	{
		if(breg > 0)
		{
			if(dreg == 0) 
				PC = EffAddress(breg, address);
			else 
				PC = address;
		}
		else
			;
	}
	
	//BLZ
	//Branches to an address when content of B-reg < 0
	void BLZ(int breg, int dreg, int address) 
	{
		if(breg < 0) 
		{
			if(dreg == 0) 
				PC = EffAddress(breg, address);
			else 
				PC = address;
		}
		else
			;
	}
	
	//effective address
	//takes int value and address
	int EffAddress(int offset, int address) 
	{
		return offset + address;
	}
    int PC; //Address or array index of the instruction in memory
    std::string registers[16];
private:
    Memory* mem;
};
};


#endif //SIMULATOR_CPU_H
