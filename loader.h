#ifndef SIMULATOR_LOADER_H
#define SIMULATOR_LOADER_H

#include <string>
#include <fstream>
#include <sstream>
#include "Structs.h"
#include <queue>

class Loader
{
public:
    Loader(std::string file, Memory memory, PCB PCB1);
	
	//methods
	void readFromFile(std::string filename, Memory memory, PCB PCB1);
	
};


#endif //SIMULATOR_LOADER_H