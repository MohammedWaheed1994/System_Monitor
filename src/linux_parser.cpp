#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <experimental/filesystem>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::stoi;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// using filesystem to handle reading Pid.
namespace fs = std::experimental::filesystem;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  for (auto SubbFolder : fs::directory_iterator(kProcDirectory)) {
    string FolderName = SubbFolder.path().filename();
    if (std::all_of(FolderName.begin(), FolderName.end(), isdigit)) {
      int pid = stoi(FolderName);
      pids.push_back(pid);
    }
  }

  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  float MemTotal;
  float MemFree;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          MemTotal = stof(value);
        } else if (key == "MemFree") {
          MemFree = stof(value);
        }
      }
    }
  }
  return (MemTotal - MemFree) / MemTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string UpTimeINsec;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> UpTimeINsec;
  }
  return stol(UpTimeINsec);
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> CpuData;
  string line;
  string CpuInfo;
  long jiffies{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> CpuInfo;
    if (CpuInfo == "cpu") {
      while (linestream >> CpuInfo) {
        jiffies += stol(CpuInfo);
      }
    }
  }
  return jiffies;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string time;
  long ActiveJiffies{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int counter = 0; counter < 17; counter++) {
      linestream >> time;
      if (counter > 12) ActiveJiffies += stol(time);
    }
  }
  return ActiveJiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> CpuData = LinuxParser::CpuUtilization();
  long Activejiffies{0};

  Activejiffies = stol(CpuData[0]) + stol(CpuData[1]) + stol(CpuData[2]) +
                  stol(CpuData[5]) + stol(CpuData[6]) + stol(CpuData[7]);
  return Activejiffies;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> CpuData = LinuxParser::CpuUtilization();

  return stol(CpuData[3]);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> CpuData;
  string line;
  string CpuInfo;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> CpuInfo;
    if (CpuInfo == "cpu") {
      while (linestream >> CpuInfo) {
        CpuData.push_back(CpuInfo);
      }
    }
  }
  return CpuData;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string KeyName, MemValue;
  string line;
  int MemValueInMega{0};
  std::ifstream filestreamStatus(kProcDirectory + to_string(pid) +
                                 kStatusFilename);
  if (filestreamStatus.is_open()) {
    while (std::getline(filestreamStatus, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> KeyName >> MemValue) {
        if (KeyName == "VmSize") {
          MemValueInMega = stoi(MemValue) / 1024;
          return to_string(MemValueInMega);
        }
      }
    }
  }

  return to_string(MemValueInMega);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string KeyName, userID;
  string line;
  std::ifstream filestreamStatus(kProcDirectory + to_string(pid) +
                                 kStatusFilename);
  if (filestreamStatus.is_open()) {
    while (std::getline(filestreamStatus, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> KeyName >> userID) {
        if (KeyName == "Uid") {
          return userID;
        }
      }
    }
  }
  return userID;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string dummy, UserName, user;
  int UID = stoi(LinuxParser::Uid(pid));
  string line;

  std::ifstream filestreamPasswd(kPasswordPath);

  if (filestreamPasswd.is_open()) {
    while (std::getline(filestreamPasswd, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> UserName >> dummy >> user;
      if (stoi(user) == UID) {
        return UserName;
      }
    }
  }

  return UserName;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  string uptime{"0"};
  long uptimeLong{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int counter = 0; counter < 14; counter++) {
      linestream >> uptime;
    }
  }

   uptimeLong = stol(uptime) / sysconf(_SC_CLK_TCK);
  return uptimeLong;
}
