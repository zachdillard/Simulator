#ifndef PROJECT_SHORTTERM_H
#define PROJECT_SHORTTERM_H

#include <queue>
#include "CPU.h"
#include "Memory.h"
#include "PriorityQueue.h"
#include <time.h>
class ShortTerm {
//Get top value of ready_queue
    //Thats the PID
    //Go to PCB that equals the PID and get the PC value of the proccess
    //Put that value into the PC of the CPU
public:
    ShortTerm() {};
    //std::queue<int> ready_queue;
    PriorityQueue ready_queue;
    std::queue<int> io_waiting_queue;
    std::queue<int> page_waiting_queue;
    void dispatch(Memory* memory, CPU* cpu)
    {
        int pid = ready_queue.front();
        cpu->setRamStart(memory->pcbs[pid]->ramStart);
        cpu->setProcessLength(memory->pcbs[pid]->processLength);
        cpu->setProcessID(pid);
        memcpy(cpu->registers, memory->pcbs[pid]->registers, sizeof(cpu->registers));
        memcpy(cpu->cache, memory->pcbs[pid]->cache, sizeof(cpu->cache));
        cpu->PC = memory->pcbs[pid]->csPC;
        memory->pcbs[pid]->status = "running";
        memory->pcbs[pid]->percentCache = cpu->percentCacheUsed();
        cpu->jobs.push_back(pid);
        ready_queue.pop();
    };
    void ioContextSwitch(Memory* memory, CPU* cpu)
    {
        int pid = cpu->getProcessID();
        memcpy(memory->pcbs[pid]->registers, cpu->registers, sizeof(cpu->registers));
        memcpy(memory->pcbs[pid]->cache, cpu->cache, sizeof(cpu->cache));
        memory->pcbs[pid]->csPC = cpu->PC;
        memory->pcbs[pid]->status = "waiting";
        memory->pcbs[pid]->ioRequests++;
        memory->pcbs[pid]->waitingClock = clock();
        cpu->running = false;
        io_waiting_queue.push(pid);
    }
    void pageContextSwitch(Memory* memory, CPU* cpu)
    {
        int pid = cpu->getProcessID();
        memcpy(memory->pcbs[pid]->registers, cpu->registers, sizeof(cpu->registers));
        memcpy(memory->pcbs[pid]->cache, cpu->cache, sizeof(cpu->cache));
        memory->pcbs[pid]->csPC = cpu->PC;
        memory->pcbs[pid]->status = "waiting";
        memory->pcbs[pid]->pageFaultCount++;
        memory->pcbs[pid]->waitingClock = clock();
        cpu->running = false;
        page_waiting_queue.push(pid);
    }
private:
    Memory* memory;
};
#endif //PROJECT_SHORTTERM_H
