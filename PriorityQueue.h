#ifndef PriorityQueue_h
#define PriorityQueue_h

#include <stdio.h>
#include <vector>
#include <iostream>
#include "Memory.h"

using namespace std;


class PriorityQueue
{
public:
    unsigned long size()
    {
        return queue.size();
    }
    
    bool empty()
    {
        return size() == 0;
    }
    
    void push(PCB* key)
    {
        queue.push_back(key);
        int index = size() - 1;
        heapify_up(index);
    }
    
    void pop()
    {
        queue[0] = queue.back();
        queue.pop_back();
        heapify_down(0);
    }

    int front()
    {
        return queue[0]->id;
    }
    
private:
    vector<PCB*> queue;
    
    int parent(int i)
    {
        return (i - 1) / 2;
    }
    
    int left_child(int i)
    {
        return (2 * i + 1);
    }
    
    int right_child(int i)
    {
        return (2 * i + 2);
    }
    
    void heapify_down(int i)
    {
        int left = left_child(i);
        int right = right_child(i);
        
        int smallest = i;
        if (left < size() && queue[left]->priority < queue[i]->priority)
            smallest = left;
        
        if (right < size() && queue[right]->priority < queue[smallest]->priority)
            smallest = right;
        
        if (smallest != i)
        {
            swap(queue[i], queue[smallest]);
            heapify_down(smallest);
        }
    }
    
    void heapify_up(int i)
    {
        if (i && queue[parent(i)]->priority > queue[i]->priority)
        {
            swap(queue[i], queue[parent(i)]);
            heapify_up(parent(i));
        }
    }
};

#endif /* idQueue_hpp */
