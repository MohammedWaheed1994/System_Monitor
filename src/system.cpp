#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Use compare processes to sort the process in ascending order.
bool CompareProcesses(Process P1, Process P2) {
  if (P1 < P2)
    return true;
  else
    return false;
}

// Return a container composed of the system's processes.
vector<Process>& System::Processes() {
  // Clear stored processes stack.
  processes_.clear();

  // Get list of Pid os the system.
  vector<int> PIDS = LinuxParser::Pids();

  // Add the Pids to current stack of processes.
  for (auto PID : PIDS) {
    processes_.push_back(Process(PID));
  }

  // Sort processes stack in ascending order.
  sort(processes_.begin(), processes_.end(), CompareProcesses);

  // Reverse the stack order to be in descending order.
  std::reverse(processes_.begin(), processes_.end());
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
