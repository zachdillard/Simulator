#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H
#include <string>
#include "Memory.h"
#include <bitset>
#include <sstream>


using namespace std;


class CPU {
public:
    //FETCH
    //Go to the PC and get its value, this is the address of the instruction
    //Go to that address and retrieve the value
    //Update PC by 1
    unsigned int fetch(){
        unsigned int memValue = cache[PC];
        PC += 4;
	return memValue;
    }
    //DECODE
    //Read each bit to figure out if I/O instruction or Compute
    //Figure out the opcode
    void decoder(unsigned int memValue){
        //Converts hex input to binary
        bitset<32> ourValue(memValue);
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
        return decodedValue.substr(2,6);
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
            case 0x00:
                ArithmeticFormat(statement);
                break;
            case 0x1:
                ConditionalBranch(statement);
                break;
            case 0x2:
                UnconditionalJump(statement);
                break;
            case 0x3:
                IOformat(statement);
                break;
        }
    }
    //One of these registers could be the Program Counter
    //OR maybe create a variable called PC
    //The project specifications states that the first to registers are special purpose
    //For now let's have it has a public variable so the Short Term scheduler can access it easily
    
    
    
    CPU(Memory* memory)
    {
        mem = memory;
        fill_n(registers, 16, 0);
    }
    
    void IOformat(string statement)
    {
        int reg1;
        int reg2;
        int address;
        int opcode;
        
        opcode = stoul(getOpCode(statement), nullptr, 2);
        reg1 = stoul(statement.substr(8, 4), nullptr, 2);
        reg2 = stoul(statement.substr(12, 4), nullptr, 2);
        address = stoul(statement.substr(16, 16), nullptr, 2);
        
        
        switch(opcode)
        {
            case 0x0:
                RD(reg1, reg2, address);
                break;
            case 0x1:
                WR(reg1, reg2, address);
                break;
        }
    }
    void UnconditionalJump(string statement)
    {
        int opcode = stoul(getOpCode(statement), nullptr, 2);
        int address = stoul(statement.substr(16,16), nullptr, 2);
        
        switch(opcode)
        {
            case 0x12:
                HLT();
                break;
            case 0x14:
                JMP(address);
                break;
        }
    }
    void ConditionalBranch(string statement)
    {
        int breg = stoul(statement.substr(8, 4), nullptr, 2);
        int dreg = stoul(statement.substr(12, 4), nullptr, 2);
        int address = stoul(statement.substr(16, 16), nullptr, 2);
        int opcode = stoul(getOpCode(statement), nullptr, 2);
        
        switch (opcode) {
            case 0x02:
                ST(breg, dreg, address);
                break;
            case 0x03:
                LW(breg, dreg, address);
                break;
            case 0x0b:
                MOVI(breg, dreg, address);
                break;
            case 0x0c:
                ADDI(breg, dreg, address);
                break;
            case 0x0d:
                MULI(breg, dreg, address);
                break;
            case 0x0f:
                LDI(breg, dreg, address);
                break;
            case 0x11:
                SLTI(breg, dreg, address);
                break;
            case 0x15:
                BEQ(breg, dreg, address);
                break;
            case 0x16:
                BNE(breg, dreg, address);
                break;
            case 0x17:
                BEZ(breg, dreg, address);
                break;
            case 0x18:
                BNZ(breg, dreg, address);
                break;
            case 0x19:
                BGZ(breg, dreg, address);
                break;
            case 0x1a:
                BLZ(breg, dreg, address);
                break;
        }
    }
    void ArithmeticFormat(string statement)
    {
        int sreg1 = stoul(statement.substr(8, 4), nullptr, 2);
        int sreg2 = stoul(statement.substr(12, 4), nullptr, 2);
        int dreg = stoul(statement.substr(16, 4), nullptr, 2);
        int opcode = stoul(getOpCode(statement), nullptr, 2);
        
        switch(opcode)
        {
            case 0x04:
                MOV(sreg1, sreg2, dreg);
                break;
            case 0x05:
                ADD(sreg1, sreg2, dreg);
                break;
            case 0x06:
                SUB(sreg1, sreg2, dreg);
                break;
            case 0x07:
                MUL(sreg1, sreg2, dreg);
                break;
            case 0x08:
                DIV(sreg1, sreg2, dreg);
                break;
            case 0x09:
                AND(sreg1, sreg2, dreg);
                break;
            case 0x0a:
                OR(sreg1, sreg2, dreg);
                break;
            case 0x10:
                SLT(sreg1, sreg2, dreg);
                break;
        }
    }
	
    //Read
	//Reads the content of I/P buffer into an accumulator
    void RD(int reg1, int reg2, int address)
    {
        if(reg2 == 0)
            registers[reg1] = cache[address];
        else
            registers[reg1] = cache[registers[reg2]];
    }
	//write 
	//Writes the content of accumulator into O/P buffer
    void WR(int reg1, int reg2, int address)
	{
		if(reg2 == 0)
            cache[address] = registers[reg1];
        else
            cache[registers[reg2]] = registers[reg1];
	}
	
	//store
	//stores the content of a register into an address
	void ST(int breg, int dreg, int address)
	{
        cache[registers[dreg] + address] = registers[breg];
	}
	
	//load
	//loads the content of an address into a register
	void LW(int breg, int dreg, int address)
	{
        registers[dreg] = cache[registers[breg] + address];
	}
	
	//move
	//transfers the content of one register into another
	void MOV(int sreg1, int sreg2, int dreg)
	{
        registers[sreg1] = registers[sreg2];
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
	
	//Logical AND of two S-regs into D-reg
	void AND(int sreg1, int sreg2, int dreg) 
	{
        registers[dreg] = registers[sreg1] & registers[sreg2];
	}
	
	//Logical OR of two S-regs into D-reg
	void OR(int sreg1, int sreg2, int dreg) 
	{
        registers[dreg] = registers[sreg1] | registers[sreg2];
	}
	
	//MOVI
	//Transfers addresses directly into a register
	void MOVI(int breg, int dreg, int address) 
	{
		if(breg == 0)
            registers[dreg] = address;
		else
            registers[dreg] = cache[registers[breg] + address];
	}
	
	//ADDI
	//Adds a data value directly to the content of a register
	void ADDI(int breg, int dreg, int address)
	{
		if(breg == 0)
            registers[dreg] += address;
		else
            registers[dreg] += cache[registers[breg] + address];
	}
	
	//MULI
	//Multiplies a data value directly with the content of a register
	void MULI(int breg, int dreg, int address)
	{
        if(breg == 0)
            registers[dreg] *= address;
        else
            registers[dreg] *= cache[registers[breg] + address];
    }
	
	//DIVI
	//Divides a data value directly to the content of a register
	void DIVI(int breg, int dreg, int address)
	{
        if(breg == 0)
            registers[dreg] /= address;
        else
            registers[dreg] /= cache[registers[breg] + address];
	}
	
	//LDI
	//Loads a data/address directly to the content of a register
	void LDI(int breg, int dreg, int address) 
	{
		if(breg == 0)
            registers[dreg] = address;
        else
            registers[dreg] = cache[registers[breg] + address];
	}
	
	//SLT
	//Sets the D-reg to 1 if first S-reg is less than the B-reg; 0 otherwise
	void SLT(int sreg1, int sreg2, int dreg)
	{
		if(registers[sreg1] < registers[sreg2])
			registers[dreg] = 1;
		else
			registers[dreg] = 0;
	}
	
	//SLTI
	//Sets the D-reg to 1 if first S-reg is less than a piece of data; 0 otherwise
	void SLTI(int breg, int dreg, int address)
	{
		if(registers[breg] < address)
            registers[dreg] = 1;
        else
            registers[dreg] = 0;
	}
	
	//HLT
	//Logical end of Program
	void HLT() 
	{
        PC = 0;
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
		PC = address;
	}
	
	//BEQ
	//Branches to an address when content of B-reg = D-reg
	void BEQ(int breg, int dreg, int address) 
	{
		if(registers[breg] == registers[dreg])
            PC = address;
	}
	
	//BNE
	//Branches to an address when content of B-reg != D-reg
	void BNE(int breg, int dreg, int address) 
	{
        if(registers[breg] != registers[dreg])
            PC = address;
	}
	
	//BEZ
	//Branches to an address when content of B-reg = 0
	void BEZ(int breg, int dreg, int address) 
	{
        if(registers[breg] == 0)
            PC = address;
	}
	
	//BNZ
	//Branches to an address whne content of B-reg != 0
	void BNZ(int breg, int dreg, int address) 
	{
        if(registers[breg] != 0)
            PC = address;
	}
	
	//BGZ
	//Branches to an address when content of B-reg > 0
	void BGZ(int breg, int dreg, int address) 
	{
        if(registers[breg] > 0)
            PC = address;
	}
	
	//BLZ
	//Branches to an address when content of B-reg < 0
	void BLZ(int breg, int dreg, int address) 
	{
        if(registers[breg] < 0)
            PC = address;
	}
	
	//effective address
	//takes int value and address
	int EffAddress(int offset, int address) 
	{
		return offset + address;
	}
    
    void setRamStart(int address)
    {
        ramStart = address;
    }
    
    void setProcessLength(int length)
    {
        processLength = length;
    }
    
    void clearCache()
    {
        for(int i = 0; i < processLength * 4; ++i)
            cache[i] = 0;
    }
     
    int PC; //Address or array index of the instruction in memory
    unsigned int registers[16];
    unsigned int cache[400];
private:
    Memory* mem;
    int ramStart;
    int processLength;
    int ID;
};


#endif //SIMULATOR_CPU_H
