#ifndef SIMULATOR_SCHEDULERS_H
#define SIMULATOR_SCHEDULERS_H

#include <queue>
#include "Memory.h"
#include "CPU.h"
#include "ShortTerm.h"

class LongTerm {
public:
    LongTerm(Memory& memory, ShortTerm& shortTerm)
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
        return mem.pcbs[getCurrentPID()];
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
        int ramPC = getCurrentProcess()->ramStart;
        for(int i = getProcessStartInDisk(); i < getProcessLength(); ++i)
        {
            mem.setRAM(ramPC, mem.getDisk(i));
            ++ramPC;
        }
        nextRAMStart = getProcessLength();
        mem.ramCount += getProcessLength();
        addToQueue();
    };
    void setProcessCount(int count) {processCount = count;}
private:
    Memory mem;
    ShortTerm ss;
    int processCount;
    int nextRAMStart;
    void addToQueue()
    {
        ss.ready_queue.push(processCount);
        ++processCount;
    };
};

#endif //PROJECT_SCHEDULERS_H