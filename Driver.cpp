#include <iostream>
#include <string>
#include "LongTerm.h"
#include "ShortTerm.h"
#include "Memory.h"
#include "Loader.h"

int main() {
    Memory memory;
    Loader loader;
    CPU cpu;
    ShortTerm shortTerm;
    //Change this to the absolute location of where you're storing the program file
    loader.readFromFile("/Users/zachdillard/Simulator/file.txt", &memory);
    LongTerm longTerm(&memory, &shortTerm);
    while(longTerm.getProcessLength() < memory.ramSpaceLeft())
        longTerm.addToRam();
    //shortTerm.dispatch(cpu);
    return 0;
}
