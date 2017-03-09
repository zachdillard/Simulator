#include <iostream>
#include <string>
#include "LongTerm.h"
#include "ShortTerm.h"
#include "Memory.h"
#include "Loader.h"
#include "CPU.h"
#include "ShortLoader.h"
#include "Clock.h"

int main() {
    Memory memory;
    Loader loader;
    int cpuCount = 4;
    CPU* cpus[cpuCount];
    for(int i = 0; i < cpuCount; ++i)
        cpus[i] = new CPU(&memory);
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
        //longTerm.sortList();
        for(int i = 0; i < longTerm.sort_queue.size(); ++i)
            shortTerm.ready_queue.push(longTerm.sort_queue[i]->id);
        while(shortTerm.ready_queue.size() > 0 || cpus[0]->running || cpus[1]->running || cpus[2]->running || cpus[3]->running)
        {
            for(int i = 0; i < cpuCount; ++i)
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
                    if(cpus[i]->running || shortTerm.ready_queue.size() != 0) //Courtesy of Will
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
        longTerm.sort_queue.clear();
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
