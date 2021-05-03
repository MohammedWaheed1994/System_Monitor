#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Initialize Pid and Cpu utilization of each process.
Process::Process(int x) : PID(x) { ProcessUtilization = CpuUtilization(); };

// Return this process's ID
int Process::Pid() { return PID; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
  long TotalTime = LinuxParser::ActiveJiffies(PID);
  long StartTime = this->UpTime();
  long Seconds = (LinuxParser::UpTime() - StartTime);
  float ProcessUtilizationValue =
      (float)TotalTime / sysconf(_SC_CLK_TCK) / Seconds;

  return ProcessUtilizationValue;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(PID); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(PID); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(PID); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(PID); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return this->ProcessUtilization < a.ProcessUtilization;
}
