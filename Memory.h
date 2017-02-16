#include <string>

struct PCB {
    int id;         //process ID
    int size;       //Number of instructions
    int pc;         //program counter
    int status;     //new, ready, waiting, etc.
    int buff_in;    //start address of input buffer
    int buff_out;   //start address of output buffer
    int buff_temp;  //start address of temporary buffer
};

class Memory 
{
public:
	int* pcbs[40];
	void setRAM(int address, std::string value);
	void setDisk(int address, std::string value);
	std::string getRAM(int address);
	std::string getDisk(int address);
private:
	std::string disk[2048];
	std::string ram[1024];
};