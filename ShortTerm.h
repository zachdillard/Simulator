#ifndef PROJECT_SHORTTERM_H
#define PROJECT_SHORTTERM_H

#include <queue>
#include "CPU.h"
#include "Memory.h"
#include "PriorityQueue.h"

class ShortTerm {
//Get top value of ready_queue
    //Thats the PID
    //Go to PCB that equals the PID and get the PC value of the proccess
    //Put that value into the PC of the CPU
public:
    ShortTerm() {};
    //std::queue<int> ready_queue;
    PriorityQueue ready_queue;
    void dispatch(Memory* memory, CPU* cpu)
    {
        cpu->setRamStart(memory->pcbs[ready_queue.front()]->ramStart);
        cpu->setProcessLength(memory->pcbs[ready_queue.front()]->processLength);
        cpu->setProcessID(memory->pcbs[ready_queue.front()]->id);
        memory->pcbs[ready_queue.front()]->percentCache = cpu->percentCacheUsed();
        cpu->jobs.push_back(memory->pcbs[ready_queue.front()]->id);
        ready_queue.pop();
    };
private:
    Memory* memory;
};
#endif //PROJECT_SHORTTERM_H
