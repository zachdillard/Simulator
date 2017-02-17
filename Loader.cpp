#include <iostream>
#include <fstream>
#include "Structs.h"
#include "loader.h"
using namespace std;

class Loader 
{
	void readFromFile(string filename, Memory memory)
	{
		ifstream infile("file.txt");
		string line;
		int PCBcount = 0;
		int pcbs[PCBcount];
		int PCBpriority;
		int lengthOfJob;
		int ID;
		int processNum;
		int inBuffer;
		int outBuffer;
		int tempBuffer;
		bool instruction = true;
		while getline(infile, line)
		{
			int diskCount = 0;
			
			if(line.substr(0,6) == "// JOB")
			{
				istringstream jobData(line);
				string infoArray[5];
				int count = 0;
				while (count < 5)
					string temp1 = "";
					string temp2 = "0x";
					string temp3 = "";
					jobData >> temp1;
					temp3 = temp2 + temp1;
					infoArray[count] = temp3;
					count++;
				}
			ID =infoArray[2];
			lengthOfJob = infoArray[3];
			PCBpriority = infoArray[4];
			
			instruction = true;
			memory.pcbs[PCBcount] = new PCB;
			memory.pcbs[PCBcount]->id = ID;
			memory.pcbs[PCBcount]->size = lengthOfJob;
			memory.pcbs[PCBcount]->priority = PCBpriority;
			
			
			else if( line.substr(0,2) == "0x")
			{
				istringstream instructData(line);
				string instructTemp = line.substr(0,8);
				memory[diskCount] = instructTemp;
				diskCount++;
			}
			
			else if(line.substr(0,7) == "// DATA")
			{
				istringstream dataData(line);
				string infoArray[5];
				int count = 0;
				while (count < 5)
					string temp1 = "";
					string temp2 = "0x";
					string temp3 = "";
					dataData >> temp1;
					temp3 = temp2 + temp1;
					infoArray[count] = temp3;
					count++;
				}
			inBuffer = infoArray[2];
			outBuffer = infoArray[3];
			tempBuffer = infoArray[4];
			
			memory.pcbs[PCBcount]->buff_in = inBuffer;
			memory.pcbs[PCBcount]->buff_out = outBuffer;
			memory.pcbs[PCBcount]->buff_temp = tempBuffer;
			}
		}
		
	}
