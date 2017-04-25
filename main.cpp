/*
 Phase 1
 Operating Systems
 
 Andrew Owen
 William Todd
 Brett Warner
 Zach Dillard
 */

#include <iostream>
#include <string>
#include "LongTerm.h"
#include "ShortTerm.h"
#include "Memory.h"
#include "Loader.h"
#include "CPU.h"
#include "ShortLoader.h"
#include "DMA.h"
#include "MMU.h"
#include <time.h>
#include <array>
#include "Queue.h"

//THIS IS FOR 4 CPUS USING A PRIORITY QUEUE

//ATTEMPTING CONTEXT SWITCH

int main() {
    //Set up each module
    Memory memory;
    Loader loader;
    int const cpuCount = 1;
    CPU* cpus[cpuCount];
    for(int i = 0; i < cpuCount; ++i)
        cpus[i] = new CPU(&memory);
    ShortTerm shortTerm;
    ShortLoader shortLoader(&memory, &shortTerm);
    DMA dma(&memory, &shortTerm);
    MMU mmu(&memory, &shortTerm);
    //Change this to the absolute location of where you're storing the program file
    //Under Windows operating systems, you should be able to just use programfile.txt as the string
    //as long as programfile.txt is located in the same folder as the source files.
    //Loads entire program file into disk array
    loader.readFromFile("/Users/zachdillard/School/OperatingSystems/Simulator/Simulator/programfile.txt", &memory);
    LongTerm longTerm(&memory, &shortTerm, &mmu);
    
    //This should run twice since we're loading half the jobs
    while(longTerm.getProcessCount() <= memory.JobCount)
    {
        longTerm.setNextRamStart(0);
        //Load the first 15 processes to RAM
        while(longTerm.getProcessCount() <= memory.JobCount && longTerm.getProcessLength() < memory.ramSpaceLeft())
            longTerm.addToRam();
        //While the ready queue still has processes AND if any CPUs are still running
        while(shortTerm.ready_queue.size() > 0 || cpus[0]->running /*|| cpus[1]->running || cpus[2]->running || cpus[3]->running*/)
        {
            //Cycle through each CPU
            for(int i = 0; i < cpuCount; ++i)
            {
                //If it's not running, assign it the top process in the ready queue
                if(cpus[i]->running == false && shortTerm.ready_queue.size() != 0)
                {
                    //Tell the CPU where the prcoess starts in RAM
                    cpus[i]->setRamStart(memory.pcbs[shortTerm.ready_queue.front()]->ramStart);
                    //Set the CPU ID for the process running on it in its PCB
                    memory.pcbs[shortTerm.ready_queue.front()]->cpuID = i;
                    shortLoader.setLengths();
                    
                    //Fill the PCBs cache with its instructions and I/O buffers
                    if(memory.pcbs[shortTerm.ready_queue.front()]->status == "new")
                        shortLoader.fillCache(&memory, &shortTerm);
                    
                    //Timing
                    memory.pcbs[shortTerm.ready_queue.front()]->waitingClock = clock() - memory.pcbs[shortTerm.ready_queue.front()]->waitingClock;
                    memory.pcbs[shortTerm.ready_queue.front()]->totalWaitingCycles += memory.pcbs[shortTerm.ready_queue.front()]->waitingClock;
                    memory.pcbs[shortTerm.ready_queue.front()]->runningClock = clock();
                    if(memory.pcbs[shortTerm.ready_queue.front()]->status != "new")
                    {
                        memory.pcbs[shortTerm.ready_queue.front()]->contextSwitchClock = clock() - memory.pcbs[shortTerm.ready_queue.front()]->contextSwitchClock;
                        memory.pcbs[shortTerm.ready_queue.front()]->totalWaitingCycles += memory.pcbs[shortTerm.ready_queue.front()]->contextSwitchClock;
                    }
                    //Dispatch
                    shortTerm.dispatch(&memory, cpus[i]);
                    cpus[i]->running = true;
                }
                //If the current CPU does have a process running on it
                else
                {
                    //If there is at least one CPU running
                    if(cpus[i]->running || shortTerm.ready_queue.size() != 0) //Courtesy of Will
                    {
                        //if not found in memory
                        //pass to memory manager
                        //have mmu determine page number and page off set
                        //go to the index in process's pcb that's equal to the page number
                        //context switch
                        unsigned int tempPC = cpus[i]->PC / 4;
                        unsigned int pageValue = cpus[i]->PC >> 2; //get all but the last 2 bits;
                        unsigned int offset = cpus[i]->PC & 0x3;
                        
                        if(memory.pcbs[cpus[i]->getProcessID()]->pagetable[pageValue].valid == false)
                        {
                            memory.pcbs[cpus[i]->getProcessID()]->runningClock = clock() - memory.pcbs[cpus[i]->getProcessID()]->runningClock;
                            memory.pcbs[cpus[i]->getProcessID()]->totalRunningCycles += memory.pcbs[cpus[i]->getProcessID()]->runningClock;
                            memory.pcbs[cpus[i]->getProcessID()]->contextSwitchClock = clock();
                            memory.pcbs[cpus[i]->getProcessID()]->currentPage = pageValue;
                            shortTerm.pageContextSwitch(&memory, cpus[i]);
                        }
                        
                        else
                        {
                            //Check the address
                            //int address = cpus[i]->PC; // 0000 1111 01
                            //Determines if the current instruction is an I/O request
                            unsigned int memValue = cpus[i]->fetch();
                            bitset<32> ourValue(memValue);
                            string decodedValue = ourValue.to_string();
                            string instructionType = decodedValue.substr(0, 2);
                            unsigned int instrType = stoul(instructionType, nullptr, 2);
                            
                            
                            //If it is, designated by value of 0x3, block the process
                            if(instrType == 0x3 && memory.pcbs[cpus[i]->getProcessID()]->ioGranted == false)
                            {
                                //Because our fetch function updates the PC value by 4
                                //It must be subtracted before the context switch or it will start
                                //at the instruction right after the IO
                                cpus[i]->PC -= 4;
                                //Take the process off the CPU and and copy the cache and registers into the PCB
                                //Then it adds the process to a waiting queue where it will remain until its IO is fullfilled
                                memory.pcbs[cpus[i]->getProcessID()]->runningClock = clock() - memory.pcbs[cpus[i]->getProcessID()]->runningClock;
                                memory.pcbs[cpus[i]->getProcessID()]->totalRunningCycles += memory.pcbs[cpus[i]->getProcessID()]->runningClock;
                                memory.pcbs[cpus[i]->getProcessID()]->contextSwitchClock = clock();
                                shortTerm.ioContextSwitch(&memory, cpus[i]);
                            }
                            
                            else
                            {
                                //If it wasn't an IO request, execute the instruction like normal
                                cpus[i]->decode(memValue);
                                
                                //The last instruction, HALT, will set CPU->running to false
                                if(cpus[i]->running == false)
                                {
                                    //Timing
                                    memory.pcbs[cpus[i]->getProcessID()]->runningClock = clock() - memory.pcbs[cpus[i]->getProcessID()]->runningClock;
                                    memory.pcbs[cpus[i]->getProcessID()]->totalRunningCycles += memory.pcbs[cpus[i]->getProcessID()]->runningClock;
                                    memory.pcbs[cpus[i]->getProcessID()]->runningTime = ((double) memory.pcbs[cpus[i]->getProcessID()]->totalRunningCycles) / CLOCKS_PER_SEC;
                                    memory.pcbs[cpus[i]->getProcessID()]->waitingTime = ((double) memory.pcbs[cpus[i]->getProcessID()]->totalWaitingCycles) / CLOCKS_PER_SEC;
                                    mmu.free(cpus[i]->getProcessID());
                                    //Copy the CPUs cache back to RAM
                                    shortLoader.toRAM(cpus[i]);
                                    //Zero out all CPU values
                                    cpus[i]->clearCPU();
                                }
                            }
                        }
                    }
                }
            }
            
            //We mimic a DMA by having it set the value of ioGranted in the PCB to true
            //Once ioGranted is true, the process can be placed back into the ready queue
            if(shortTerm.io_waiting_queue.size() > 0)
                dma.service();
            
            if(shortTerm.page_waiting_queue.size() > 0)
                if(mmu.frame_list.size() > 0)
                    mmu.service();
        }
        
        //Statistics
        memory.percentRAM.push_back(memory.percentRAMUsed());
        
        //Display output
        memory.coreDump();
        
        //Zero out RAM
        memory.clearRam();
    }
    
    //Function to print running and waiting times
    memory.printTimes();
    
    //Prints the percent of jobs that ran on each CPU
    for(int i = 0; i < cpuCount; ++i)
    {
        printf("%i,%f\n", i, cpus[i]->percentOfJobs());
    }
    return 0;
}

