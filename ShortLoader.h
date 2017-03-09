#ifndef ShortLoader_hpp
#define ShortLoader_hpp

#include <stdio.h>
#include "CPU.h"
#include "Memory.h"
#include "ShortTerm.h"
class ShortLoader
{
public:
    ShortLoader(Memory* m, ShortTerm* s)
    {
        memory = m;
        shortTerm = s;
    }
    void setLengths()
    {
        ramStart = memory->pcbs[shortTerm->ready_queue.front()]->ramStart;
        processLength = memory->pcbs[shortTerm->ready_queue.front()]->processLength;
    }
    void toCPU(CPU* cpu)
    {
        for(int i = 0; i < processLength * 4; i += 4)
            cpu->cache[i] = memory->getRAM(ramStart + i);
    }
    void toRAM(CPU* cpu)
    {
        for(int i = 0; i < processLength * 4; i += 4)
            memory->setRAM(ramStart + i, cpu->cache[i]);
    }
private:
    Memory* memory;
    ShortTerm* shortTerm;
    int ramStart;
    int processLength;
};

#endif