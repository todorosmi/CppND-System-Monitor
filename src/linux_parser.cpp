#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line1, line2;
  string key, unit;
  long int totalMem, freeMem;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line1);
    std::istringstream linestream1(line1);
    linestream1 >> key >> totalMem >> unit;
    std::getline(filestream, line2);
    std::istringstream linestream2(line2);
    linestream2 >> key >> freeMem >> unit;
  }
  return (totalMem-freeMem)/totalMem;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long upTime, idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
    return upTime;
  }
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() { 
    float cpuUtalization;
    std::string line;
    std::string cpu;
    long int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if(stream.is_open()){
        std::getline(stream,line);
        std::istringstream linestream(line);
        linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    }
    cpuUtalization = (user+nice+system)/(user+nice+system+idle);
    return cpuUtalization; 
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    string line;
    string key;
    int value;
    std::ifstream filestream(kProcDirectory+kStatFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "processes") {
            return value;
          }
        }
      }
    }
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    string line;
    string key;
    int value;
    std::ifstream filestream(kProcDirectory+kStatFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "procs_running") {
            return value;
          }
        }
      }
    }
 }

std::vector<long> LinuxParser::CpuUtilization(int pid[[maybe_unused]]) { 
    //float cpuUtalization;
    std::string line;
    std::string pidTemp, str1, str2;
    vector<long> temp(22);
    std::ifstream stream(kProcDirectory + '/' + to_string(pid) + kStatFilename);
    if(stream.is_open()){
        std::getline(stream,line);
        std::istringstream linestream(line);
        linestream >> pidTemp >> str1 >> str2 >> temp[0] >> temp[1] >> temp[2] >> temp[3] >> temp[4] >> temp[5] >> temp[6] >> temp[7] >> temp[8] >> temp[9] >> temp[10] >> temp[11] >> temp[12] >> temp[13] >> temp[14] >> temp[15] >> temp[16] >> temp[17] >> temp[18] >> temp[19] >> temp[20] >> temp[21];
    }
    //cpuUtalization = temp[13]+temp[14];
    return temp; 
 }
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { 
  std::string line;
  std::ifstream stream(kProcDirectory + '/' + to_string(pid) + kCmdlineFilename);
    if(stream.is_open()){
      std::getline(stream,line);
      return line; 
    }
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { 
  std::string line, key, unit;
  long value;
  std::ifstream stream(kProcDirectory + '/' + to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value >> unit) {
          if (key == "VmSize:") {
            return to_string(value/1000);
          }
        }
      }
    }
} 

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { 
  std::string line, key, rest;
  int value;
  std::ifstream stream(kProcDirectory + '/' + to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> key >> value >> rest;
          if (key == "Uid:") {
            return to_string(value);
          }
      }
    }
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { 
  string uid = LinuxParser::Uid(pid);
  string line, userName, strTemp, uidTemp, rest;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> userName >> strTemp >> uidTemp >> rest;
        if (uidTemp == uid) {
          return userName;
        }
    }
  }  
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { 
    std::string line;
    std::string pidTemp, str1, str2;
    vector<long> temp(22);
    std::ifstream stream(kProcDirectory + '/' + to_string(pid) + kStatFilename);
    if(stream.is_open()){
        std::getline(stream,line);
        std::istringstream linestream(line);
        linestream >> pidTemp >> str1 >> str2 >> temp[0] >> temp[1] >> temp[2] >> temp[3] >> temp[4] >> temp[5] >> temp[6] >> temp[7] >> temp[8] >> temp[9] >> temp[10] >> temp[11] >> temp[12] >> temp[13] >> temp[14] >> temp[15] >> temp[16] >> temp[17] >> temp[18] >> temp[19] >> temp[20] >> temp[21];
    }
  return temp[18]/sysconf(_SC_CLK_TCK);
; 
}
