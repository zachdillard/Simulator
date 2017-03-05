#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include <string>
#include <iostream>
#include <fstream>

struct PCB {
    int id = 0;         //process ID
    int size = 0;   //Number of instructions
    int pc = 0;         //program counter
    int diskStart = 0;
    int ramStart = 0;
    int priority = 0;
    int status = 0;     //new, ready, waiting, etc.
    int buff_in = 0;    //size of input buffer
    int buff_out = 0;   //size of output buffer
    int buff_temp = 0;  //size of temporary buffer
    int processLength = 0; //Total size of process including data/buffers
    int cpuID = 0;
};

class Memory 
{
public:
    Memory()
    {
        std::fill_n(ram, 4096, 0);
        std::fill_n(disk, 2048, 0);
    }
	PCB* pcbs[40];
    int ramCount = 0;
    int JobCount = 0;
	void setRAM(int address, unsigned int value)
    {
        ram[address] = value;
    };
	void setDisk(int address, unsigned int value)
    {
        disk[address] = value;
    };
	unsigned int getRAM(int address)
    {
        return ram[address];
    };
	unsigned int getDisk(int address)
    {
        return disk[address];
    };
    int ramSpaceUsed()
    {
        return ramCount;
    }
    int ramSpaceLeft()
    {
        return 4096 - ramCount;
    }
    void coreDump()
    {
        //DONT USE CAUSES MEMORY LEAKS
        std::ofstream file;
        file.open("/Users/zachdillard/School/OperatingSystems/Simulator/Simulator/coredump.txt", std::ios_base::app);
        for(std::size_t i = 0; i < 4096; i+=4)
        {
            file << "Addr: 0x" << std::hex << i << "\t" << ram[i]<< "\n";
        }
        file.close();
    }
    void clearRam()
    {
        for(std::size_t i = 0; i < 1024; ++i)
            ram[i] = 0;
        ramCount = 0;
    }
private:
	unsigned int disk[2048];
	unsigned int ram[4096];
};

#endif //PROJECT_MEMORY_H