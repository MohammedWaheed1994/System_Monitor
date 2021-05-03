#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

using std::string;

class Processor {
 public:
  Processor();
  float Utilization();  // See src/processor.cpp

  // Declare any necessary private members
 private:
  float CpuValue;

  //Variables to store current data of Cpu since Monitor started to calculate 
  //Cpu load in more efficient way.
  float PrevNonIdle;
  float PrevTotal;
};

#endif