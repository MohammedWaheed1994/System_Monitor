#include "processor.h"

#include <vector>

#include "linux_parser.h"

using std::stof;
using std::vector;

// Constructor to get the start value of system jiffies
Processor::Processor() {
  PrevNonIdle = LinuxParser::ActiveJiffies();
  PrevTotal = LinuxParser::IdleJiffies() + LinuxParser::ActiveJiffies();
}

// Return the aggregate CPU utilization
float Processor::Utilization() {
  float CpuValue{0};
  float Total{};

  float CurrentNonIdle = LinuxParser::ActiveJiffies();
  float CurrentTotal =
      LinuxParser::IdleJiffies() + LinuxParser::ActiveJiffies();

  float NonIdle = CurrentNonIdle - PrevNonIdle;

//Safety check that difference is not 0, so that not to fall into system exception.
  if ((CurrentTotal - PrevTotal) == 0)
    Total = CurrentTotal;

  else
    Total = CurrentTotal - PrevTotal;

  CpuValue = NonIdle / Total;

  return CpuValue;
}