#ifndef SIMULATOR_LONGTERM_H
#define SIMULATOR_LONGTERM_H

#include <queue>
#include "Memory.h"
#include "CPU.h"
#include "ShortTerm.h"

class LongTerm {
public:
    LongTerm(Memory *memory, ShortTerm *shortTerm)
    {
        mem = memory;
        ss = shortTerm;
        processCount = 1;
    };
    int getCurrentPID()
    {
        return processCount;
    };
    PCB* getCurrentProcess()
    {
        return mem->pcbs[getCurrentPID()];
    };
    int getProcessStartInDisk()
    {
        return getCurrentProcess()->diskStart;
    };
    int getProcessLength()
    {
        return getCurrentProcess()->processLength;
    };
    void addToRam()
    {
        getCurrentProcess()->ramStart = nextRAMStart;
        getCurrentProcess()->pc = getCurrentProcess()->ramStart;
        int diskPC = getProcessStartInDisk();
        int ramPC = getCurrentProcess()->ramStart;
        for(int i = 0; i < getProcessLength(); ++i)
        {
            mem->setRAM(ramPC, mem->getDisk(diskPC));
            ++diskPC;
            ramPC += 4;
            mem->ramCount += 4;
        }
        nextRAMStart = getCurrentProcess()->ramStart + (getProcessLength() * 4);
        addToQueue();
    };
    void setNextRamStart(int index) {nextRAMStart = index;}
    int getProcessCount() {return processCount;}
private:
    Memory* mem;
    ShortTerm* ss;
    int processCount;
    int nextRAMStart;

    void addToQueue()
    {
        ss->ready_queue.push(processCount);
        ++processCount;
    };
    /*vector<PCB*> sort_queue;
    bool compareByPriority(const PCB *a, const PCB *b)
    {
        return a->priority < b->priority;
    }
    void sortQueue()
    {
        sort_queue.push_back(mem->pcbs[processCount]);
        std::sort(sort_queue.begin(), sort_queue.end(), compareByPriority);
    }*/
   
};

#endif //PROJECT_LONGTERM_H
