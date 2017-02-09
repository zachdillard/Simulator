#ifndef SIMULATOR_LOADER_H
#define SIMULATOR_LOADER_H

#include <string>
#include <fstream>
#include "Structs.h"
#include <queue>

class Loader
{
public:
    Loader(std::string file, Memory memory, std::queue new_queue);
};


#endif //SIMULATOR_LOADER_H
