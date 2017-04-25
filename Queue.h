#ifndef Queue_h
#define Queue_h

#include <stdio.h>
#include <vector> 
#include <cassert>

class Queue
{
public:
    
    int size()
    {
        return count;
    }
    
    bool empty()
    {
        return size() == 0;
    }
    
    void push(int value)
    {
        queue.push_back(value);
        count++;
    }
    
    void pop()
    {
        assert(size() != 0);
        first++;
        count--;
    }
    
    int front()
    {
        return queue[first];
    }
    
private:
    vector<int> queue;
    int count = 0;
    int first = 0;
};

#endif
