#ifndef DMA_h
#define DMA_h

#include <stdio.h>
#include "Memory.h"
#include "ShortTerm.h"

class DMA
{
public:
    DMA(Memory* m, ShortTerm* s)
    {
        memory = m;
        shortTerm = s;
    }
    void service()
    {
        int pid = shortTerm->io_waiting_queue.front();
        memory->pcbs[pid]->ioGranted = true;
        shortTerm->io_waiting_queue.pop();
        shortTerm->ready_queue.push(memory->pcbs[pid]);
    }
private:
    Memory* memory;
    ShortTerm* shortTerm;
};

#endif
