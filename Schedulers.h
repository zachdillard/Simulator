#include <queue>
#include "Memory.h"
#include "CPU.h"

#ifndef SIMULATOR_SCHEDULER_H
#define SIMULATOR_SCHEDULER_H


class LongTerm {
//Takes in disk
  //Put PID into ready queue
  //Get start address of 1st process
  //Get end address
  //Load that into RAM
};

class ShortTerm {
//Get top value of ready_queue
  //Thats the PID
  //Go to PCB that equals the PID and get the PC value of the proccess
  //Put that value into the PC of the CPU
public:
  std::queue<int> ready_queue;
};


#endif //SIMULATOR_SCHEDULER_H
