#ifndef SIMULATOR_LOADER_H
#define SIMULATOR_LOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Memory.h"
#include <queue>

class Loader
{
public:
	//methods
	void readFromFile(std::string filename, Memory &memory)
    {
        int diskCount = 0;
        std::ifstream infile("file.txt");
        std::string line;
        int PCBcount = 1;
        int pcbs[PCBcount];
        int PCBpriority;
        int lengthOfJob;
        int ID;
        int processNum;
        int inBuffer;
        int outBuffer;
        int tempBuffer;
        bool instruction = true;
        while (getline(infile, line))
        {
            if(line.substr(0,6) == "// JOB")
            {
                std::istringstream jobData(line);
                std::string infoArray[5];
                int count = 0;
                while (count < 5)
                {
                    std::string temp1 = "";
                    std::string temp2 = "0x";
                    std::string temp3 = "";
                    jobData >> temp1;
                    temp3 = temp2 + temp1;
                    infoArray[count] = temp3;
                    count++;
                }
                ID = infoArray[2];
                lengthOfJob = infoArray[3];
                PCBpriority = infoArray[4];

                instruction = true;
                memory.pcbs[PCBcount] = new PCB;
                memory.pcbs[PCBcount]->id = ID;
                memory.pcbs[PCBcount]->size = lengthOfJob;
                memory.pcbs[PCBcount]->priority = PCBpriority;
            }
            else if( line.substr(0,2) == "0x")
            {
                std::istringstream instructData(line);
                std::string instructTemp = line.substr(0,8);
                memory.setDisk(diskCount, instructTemp);
                diskCount++;
            }

            else if(line.substr(0,7) == "// DATA")
            {
                std::istringstream dataData(line);
                std::string infoArray[5];
                int count = 0;
                while (count < 5)
                {
                    std::string temp1 = "";
                    std::string temp2 = "0x";
                    std::string temp3 = "";
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
};


#endif //SIMULATOR_LOADER_H