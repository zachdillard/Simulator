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
    Loader(){}
	void readFromFile(std::string filename, Memory *memory)
    {
        int diskCount = 0;
        std::ifstream infile(filename);
        std::string line;
        int PCBcount = 1;
        unsigned int PCBpriority;
        unsigned int lengthOfJob = 0;
        unsigned int ID;
        unsigned int inBuffer;
        unsigned int outBuffer;
        unsigned int tempBuffer;
        unsigned int pSize;
        
        while(std::getline(infile, line))
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
                ID = std::stoul(infoArray[2], nullptr, 16);
                lengthOfJob = std::stoul(infoArray[3], nullptr, 16);
                PCBpriority = std::stoul(infoArray[4], nullptr, 16);

                memory->pcbs[PCBcount] = new PCB;
                memory->pcbs[PCBcount]->id = ID;
                memory->pcbs[PCBcount]->size = lengthOfJob;
                memory->pcbs[PCBcount]->priority = PCBpriority;
                memory->pcbs[PCBcount]->diskStart = diskCount;
                PCBcount++;
                
            }
            else if( line.substr(0,2) == "0x")
            {
                std::istringstream instructData(line);
                std::string instructTemp = line.substr(0, 10);
                memory->setDisk(diskCount, instructTemp);
                diskCount++;
            }

            else if(line.substr(0,7) == "// Data")
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
                inBuffer = std::stoul(infoArray[2], nullptr, 16);
                outBuffer = std::stoul(infoArray[3], nullptr, 16);
                tempBuffer = std::stoul(infoArray[4], nullptr, 16);

                memory->pcbs[PCBcount - 1]->buff_in = inBuffer;
                memory->pcbs[PCBcount - 1]->buff_out = outBuffer;
                memory->pcbs[PCBcount - 1]->buff_temp = tempBuffer;
                pSize = lengthOfJob + inBuffer + outBuffer + tempBuffer;
                memory->pcbs[PCBcount - 1]->processLength = pSize;
                
            }
        }
    }
};


#endif //SIMULATOR_LOADER_H