#include <iostream>
#include <string>
#include "LongTerm.h"
#include "ShortTerm.h"
#include "Memory.h"
#include "Loader.h"
#include "CPU.h"

int main() {
    Memory memory;
    Loader loader;
    CPU cpu(&memory);
    ShortTerm shortTerm;
    //Change this to the absolute location of where you're storing the program file
    loader.readFromFile("/Users/zachdillard/School/OperatingSystems/Simulator/Simulator/programfile.txt", &memory);
    LongTerm longTerm(&memory, &shortTerm);
    while(longTerm.getProcessCount() <= memory.JobCount)
    {
        longTerm.setNextRamStart(0);
        while(longTerm.getProcessCount() <= memory.JobCount && longTerm.getProcessLength() < memory.ramSpaceLeft())
            longTerm.addToRam();
        while(shortTerm.ready_queue.size() > 0)
        {
            cpu.setRamStart(memory.pcbs[shortTerm.ready_queue.front()]->ramStart);
            shortTerm.dispatch(&memory, &cpu);
            
            do cpu.decoder(cpu.fetch());
            while(cpu.PC != 0);
            
            //Change the location of file path
        }
        memory.coreDump();
        memory.clearRam();
    }
    return 0;
}
