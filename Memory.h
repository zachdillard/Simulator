#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <time.h>

#define fhex(_v) std::setw(_v) << std::hex << std::setfill('0')

struct PCB {
    int id = 0;         //process ID
    int size = 0;   //Number of instructions
    unsigned int pc = 0;
    int diskStart = 0;
    int ramStart = 0;
    int priority = 0;
    std::string status = "new";     //new, ready, waiting, etc.
    int buff_in = 0;    //size of input buffer
    int buff_out = 0;   //size of output buffer
    int buff_temp = 0;  //size of temporary buffer
    int processLength = 0; //Total size of process including data/buffers
    clock_t waitingClock;
    clock_t runningClock;
    double waitingTime = 0;
    double runningTime = 0;
    int operations = 0;
    double percentCache = 0;
    bool ioGranted = false;
    //Context switch values
    unsigned int registers[16] = {0};
    unsigned int cache[400] = {0};
    unsigned int csPC = 0;
    int cpuID = 0;
    int pagetable[20]; // -1 represents invalid bit, any other number is a valid bit + its frame number
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
    std::vector<double> percentRAM;
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
    double percentRAMUsed()
    {
        return (double) ramSpaceUsed() / 4096.0;
    }
    void coreDump()
    {
        //DONT USE CAUSES MEMORY LEAKS
        std::ofstream file;
        file.open("/Users/zachdillard/School/OperatingSystems/Simulator/Simulator/coredump.txt", std::ios_base::app);
        file << "Address\t\tValue\n" << "----------------------\n";
        for(std::size_t i = 0; i < 4096; i+=4)
        {
            file << "0x" << fhex(8) << i << ":\t" << "0x" << fhex(8) << ram[i]<< "\n";
        }
        file << "\n";
        file.close();
    }
    
    void printTimes()
    {
        printf("Stats for processes\n");
        for(int i = 1; i <= 30; ++i)
        {
            printf("Process %i\n", i);
            std::cout << "Waiting time: " << std::setprecision (10) << pcbs[i]->waitingTime * 1000 << std::endl;
            std::cout << "Running time: " << std::setprecision (10) << pcbs[i]->runningTime * 1000 << std::endl;
            printf("IO operations: %i\n", pcbs[i]->operations);
            printf("Percent cache used: %f\n", pcbs[i]->percentCache);
            printf("Percent RAM used: %f\n", pcbs[i]->processLength / 1024.0);
            printf("\n");
        }
    }
    
    void clearRam()
    {
        for(std::size_t i = 0; i < 4096; ++i)
            ram[i] = 0;
        ramCount = 0;
    }
private:
	unsigned int disk[2048];
	unsigned int ram[4096]; //1024
};

#endif //PROJECT_MEMORY_H