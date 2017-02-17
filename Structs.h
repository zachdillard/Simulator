#ifndef SIMULATOR_STRUCTS_H
#define SIMULATOR_STRUCTS_H

struct Memory {
    std::string disk[2048];
    std::string ram[1024];
};

struct PCB {
	
    int id;         //process ID
    int size;       //Number of instructions
    int pc;         //program counter
    int status;     //new, ready, waiting, etc.
    int buff_in;    //start address of input buffer
    int buff_out;   //start address of output buffer
    int buff_temp;  //start address of temporary buffer
	int priority;	//Priority of job
};

#endif //SIMULATOR_STRUCTS_H