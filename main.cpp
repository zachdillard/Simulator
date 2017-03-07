#include <iostream>
#include <string>
#include "LongTerm.h"
#include "ShortTerm.h"
#include "Memory.h"
#include "Loader.h"
#include "CPU.h"
#include "ShortLoader.h"

int main() {
    Memory memory;
    Loader loader;
    CPU* cpus[4];
    for(int i = 0; i < 4; ++i)
        cpus[i] = new CPU(&memory);
    //CPU cpu(&memory);
    ShortTerm shortTerm;
    ShortLoader shortLoader(&memory, &shortTerm);
    //Change this to the absolute location of where you're storing the program file
    loader.readFromFile("/Users/zachdillard/School/OperatingSystems/Simulator/Simulator/programfile.txt", &memory);
    LongTerm longTerm(&memory, &shortTerm);
    while(longTerm.getProcessCount() <= memory.JobCount)
    {
        longTerm.setNextRamStart(0);
        while(longTerm.getProcessCount() <= memory.JobCount && longTerm.getProcessLength() < memory.ramSpaceLeft())
            longTerm.addToRam();
        while(shortTerm.ready_queue.size() > 0 || cpus[0]->running || cpus[1]->running || cpus[2]->running || cpus[3]->running)
        {
            //Instead of for loop just use an if statement and check each cpu every turn
            //Should cut down on idle
            for(int i = 0; i < 4; ++i)
            {
                if(cpus[i]->running == false && shortTerm.ready_queue.size() != 0)
                {
                    cpus[i]->setRamStart(memory.pcbs[shortTerm.ready_queue.front()]->ramStart);
                    memory.pcbs[shortTerm.ready_queue.front()]->cpuID = i;
                    shortLoader.setLengths();
                    shortLoader.toCPU(cpus[i]);
                    shortTerm.dispatch(&memory, cpus[i]);
                    cpus[i]->running = true;
                }
                else
                {
                    if(cpus[i]->running == true || shortTerm.ready_queue.size() != 0) //Courtesy of Will
                    {
                        cpus[i]->decoder(cpus[i]->fetch());
                        if(cpus[i]->running == false)
                        {
                            shortLoader.toRAM(cpus[i]);
                            cpus[i]->clearCPU();
                        }
                    }
                }
            }
        }
        memory.coreDump();
        memory.clearRam();
    }
    return 0;
}



/*
 
 cpu.setRamStart(memory.pcbs[shortTerm.ready_queue.front()]->ramStart);
 shortLoader.setLengths();
 shortLoader.toCPU(&cpu);
 shortTerm.dispatch(&memory, &cpu);
 
 
 do cpu.decoder(cpu.fetch());
 while(cpu.PC != 0);
 
 
 shortLoader.toRAM(&cpu);
 cpu.clearCache();
 
 */
