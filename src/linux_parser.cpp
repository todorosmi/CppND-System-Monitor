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
  string value, os;
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
          os = value;
        }
      }
    }
  }
  return os;
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
  if((float)totalMem > 0){
    return ((float)totalMem-(float)freeMem)/(float)totalMem;
  }
  return 0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long upTime = 0;
  long idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies(){ 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long totaltime;
  string line, value;
  vector<string> values;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }

  // make sure parsing was correct and values was read
  long utime = 0, stime = 0 , cutime = 0, cstime = 0;
  if (std::all_of(values[13].begin(), values[13].end(), isdigit))
    utime = stol(values[13]);
  if (std::all_of(values[14].begin(), values[14].end(), isdigit))
    stime = stol(values[14]);
  if (std::all_of(values[15].begin(), values[15].end(), isdigit))
    cutime = stol(values[15]);
  if (std::all_of(values[16].begin(), values[16].end(), isdigit))
    cstime = stol(values[16]);

  totaltime = utime + stime + cutime + cstime;
  return totaltime / sysconf(_SC_CLK_TCK);
  }
  return 0;
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  auto jiffies = CpuUtilization();

  return stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +  
  stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
  stol(jiffies[CPUStates::kSoftIRQ_]) +
  stol(jiffies[CPUStates::kSteal_]);
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  // Read and return CPU utilization
  string line, cpu, value;
  vector<string> jiffies;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    linestream >> cpu;

    while (linestream >> value) {
      jiffies.push_back(value);
    }
  }
  return jiffies;
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    string line;
    string key;
    long int value;
    int noProcesses = 0;
    std::ifstream filestream(kProcDirectory+kStatFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "processes") {
            noProcesses = value;
          }
        }
      }
    }
    return noProcesses;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    string line;
    string key;
    int value, runProcess;
    std::ifstream filestream(kProcDirectory+kStatFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "procs_running") {
            runProcess = value;
          }
        }
      }
    }
    return runProcess;
 }

std::vector<long> LinuxParser::CpuUtilization(int pid) { 
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

string LinuxParser::Command(int pid) { 
  std::string line;
  std::ifstream stream(kProcDirectory + '/' + to_string(pid) + kCmdlineFilename);
    if(stream.is_open()){
      std::getline(stream,line); 
    }
    return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::string line, key, unit;
  std::ostringstream ostream;
  long value;
  std::ifstream stream(kProcDirectory + '/' + to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value >> unit) {
          if (key == "VmSize:") {
            ostream << value/1024;
          }
        }
      }
    }
    return ostream.str();
} 

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::string line, key, rest;
  std::ostringstream ostream;
  int value;
  std::ifstream stream(kProcDirectory + '/' + to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> key >> value >> rest;
          if (key == "Uid:") {
             ostream << value;
          }
      }
    }
    return ostream.str();
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string line, userName, strTemp, uidTemp, rest, user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> userName >> strTemp >> uidTemp >> rest;
        if (uidTemp == uid) {
          user =  userName;
        }
    }
  }  
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
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
}