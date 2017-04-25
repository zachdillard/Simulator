#ifndef MMU_h
#define MMU_h

#include <stdio.h>
#include "Memory.h"
#include <queue>
#include "ShortTerm.h"
#include <vector>
#include <array>
#include "Queue.h"

class MMU
{
public:
    MMU(Memory* m, ShortTerm* s)
    {
        memory = m;
        shortTerm = s;
        for(int i = 0; i < 256; i++)
            frame_list.push(i);
    }
    
    void service()
    {
        int pid = shortTerm->page_waiting_queue.front();
        int currentPage = memory->pcbs[pid]->currentPage;
        memory->pcbs[pid]->pagetable[currentPage].frame = frame_list.front();
        memory->pcbs[pid]->pagetable[currentPage].valid = true;
        frame_list.pop();
        shortTerm->page_waiting_queue.pop();
        shortTerm->ready_queue.push(memory->pcbs[pid]);
    }
    
    void free(int pid)
    {
        PCB* p = memory->pcbs[pid];
        for(int i = 0; i < 30; i++)
            if(p->pagetable[i].valid == true)
                frame_list.push(p->pagetable[i].frame);
    }
    queue<int> frame_list;
private:
    Memory* memory;
    ShortTerm* shortTerm;
};
#endif