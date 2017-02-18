#include <iostream>
#include <string>
#include "CPU.h"
#include "Schedulers.h"
#include "ShortTerm.h"
#include "Memory.h"



int main() {
    Memory memory;
    //Loader loader;
    CPU cpu;
    ShortTerm shortTerm;
    memory.pcbs[1] = new PCB;
    memory.pcbs[1]->id = 1;
    memory.pcbs[1]->diskStart = 0;
    memory.pcbs[1]->processLength = 5;
    for(int i = 0; i < 5; ++i)
        memory.setDisk(i, "test");
    LongTerm longTerm(memory, shortTerm);
    //loader.readFromFile("programfile.txt", memory);
    longTerm.addToRam();
    //shortTerm.dispatch(cpu);
    return 0;
}
