#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include <string>

struct PCB {
    int id;         //process ID
    int size;   //Number of instructions
    int pc;         //program counter
    int diskStart;
    int ramStart;
    int priority;
    int status;     //new, ready, waiting, etc.
    int buff_in;    //start address of input buffer
    int buff_out;   //start address of output buffer
    int buff_temp;  //start address of temporary buffer
    int processLength; //Total size of process including data/buffers
    int cpuID;
};

class Memory 
{
public:
	PCB* pcbs[40];
    int ramCount;
	void setRAM(int address, std::string value)
    {
        ram[address] = value;
    };
	void setDisk(int address, std::string value)
    {
        disk[address] = value;
    };
	std::string getRAM(int address)
    {
        return ram[address];
    };
	std::string getDisk(int address)
    {
        return disk[address];
    };
    int ramSpaceUsed()
    {
        return ramCount;
    }
    int ramSpaceLeft()
    {
        return 1024 - ramCount;
    }
private:
	std::string disk[2048];
	std::string ram[1024];
};

#endif //PROJECT_MEMORY_H