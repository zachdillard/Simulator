#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include <algorithm>

class Memory
{
public:

    //Constructor
    Memory() {
        std::fill_n(registers, 16, 0);
        std::fill_n(memory, 1024, 0);
        std::fill_n(disk, 2048, 0);
    }

    //Load data from memory to a register
    void load(unsigned long addr, unsigned long reg) {
        registers[reg] = memory[addr];
    }

    //Store data from register into memory
    void store(unsigned long addr, unsigned long reg) {
        memory[addr] = registers[reg];
    }

    //Read data from disk into memory
    unsigned long read(unsigned long mem_addr, unsigned long disk_addr) {
        memory[mem_addr] = disk[disk_addr];
    }

    //Write data from memory to disk
    void write(unsigned long mem_addr, unsigned long disk_addr) {
        disk[disk_addr] = memory[mem_addr];
    }

private:
    unsigned long registers[16];
    unsigned long memory[1024];
    unsigned long disk[2048];
};


#endif //SIMULATOR_MEMORY_H
