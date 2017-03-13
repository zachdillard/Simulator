#include <iostream>
#include <string>
#include "LongTerm.h"
#include "ShortTerm.h"
#include "Memory.h"
#include "Loader.h"
#include "CPU.h"
#include "ShortLoader.h"
#include <time.h>


int main() {
    Memory memory;
    Loader loader;
    int const cpuCount = 4;
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
        /*longTerm.sortList();
        for(int i = 0; i < longTerm.sort_queue.size(); ++i)
        {
            shortTerm.ready_queue.push(longTerm.sort_queue[i]->id);
            longTerm.sort_queue[i]->waitingClock = clock();
        }*/
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
                    memory.pcbs[shortTerm.ready_queue.front()]->waitingClock = clock() - memory.pcbs[shortTerm.ready_queue.front()]->waitingClock;
                    memory.pcbs[shortTerm.ready_queue.front()]->waitingTime = ((double) memory.pcbs[shortTerm.ready_queue.front()]->waitingClock) / CLOCKS_PER_SEC;
                    memory.pcbs[shortTerm.ready_queue.front()]->runningClock = clock();
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
                            //Stop running time for processes
                            memory.pcbs[cpus[i]->getProcessID()]->runningClock = clock() - memory.pcbs[cpus[i]->getProcessID()]->runningClock;
                            memory.pcbs[cpus[i]->getProcessID()]->runningTime = ((double) memory.pcbs[cpus[i]->getProcessID()]->runningClock) / CLOCKS_PER_SEC;
                            shortLoader.toRAM(cpus[i]);
                            cpus[i]->clearCPU();
                        }
                    }
                }
            }
        }
        memory.percentRAM.push_back(memory.percentRAMUsed());
        memory.coreDump();
        memory.clearRam();
        //longTerm.sort_queue.clear();
    }
    return 0;
}
