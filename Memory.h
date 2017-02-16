#include <string>
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
}