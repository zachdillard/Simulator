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
        nextRAMStart = 0;
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
            ++ramPC;
            mem->ramCount++;
        }
        nextRAMStart = getCurrentProcess()->ramStart + getProcessLength();
        addToQueue();
    };
    void setProcessCount(int count) {processCount = count;}
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
};

#endif //PROJECT_LONGTERM_H
