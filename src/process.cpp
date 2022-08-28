#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return this->pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  long m_uptime = LinuxParser::UpTime(this->pid_);
  long seconds = LinuxParser::UpTime() - m_uptime;
  long totaltime = LinuxParser::ActiveJiffies(this->pid_);
  float m_utilization;
  try {
    m_utilization = float(totaltime) / float(seconds);

  } catch (...) {
    m_utilization = 0;
  }
  return m_utilization;
}
    

// TODO: Return the command that generated this process
string Process::Command() { 
    this->command_ = LinuxParser::Command(this->pid_);
    return this->command_; 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    this->ram_ = LinuxParser::Ram(this->pid_);
    return this->ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    string uid = LinuxParser::Uid(this->pid_);
    this->user_ = LinuxParser::User(this->pid_);
    return this->user_; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    this->upTime_ = LinuxParser::UpTime() - LinuxParser::UpTime(pid_);
    return this->upTime_; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { 
    
    if(this->cpUtalization_>a.cpUtalization_){
        return true;
    }  
    
    return false;

}