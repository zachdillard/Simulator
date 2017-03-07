#ifndef PriorityQueue_hpp
#define PriorityQueue_hpp

#include <stdio.h>
#include "Memory.h"
#include <vector>

class PriorityQueue
{
public:
    PriorityQueue()
    {
        count = 1;
    }
private:
    PCB* buffer[40];
    int count;
};

#endif