#include <iostream>
#include <string>
#include "Loader.h"
#include "CPU.h"
#include "Schedulers.h"



int main() {
    std::string text_file;
    Memory memory;
    CPU cpu;
    Long_Term long;
    Short_Term short;
    Loader loader(text_file, memory);
    return 0;
}
