#include <string>
#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H


class CPU {
public:
  //One of these registers could be the Program Counter
  //OR maybe create a variable called PC
  //The project specifications states that the first to registers are special purpose
  std::string registers[16];
};


#endif //SIMULATOR_CPU_H
