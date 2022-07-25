#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    utilization_ = LinuxParser::CpuUtilization();
    return utilization_;
 }