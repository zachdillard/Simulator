#include <iostream>
#include <string>
#include "Loader.h"
#include "Structs.h"
#include <queue>



int main() {
    std::string text_file;
    Memory memory;
    std::queue new_queue;
    Loader loader(text_file, memory, new_queue);
    return 0;
}