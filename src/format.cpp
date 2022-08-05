#include <string>
#include <cmath>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int sec = (seconds % 60);
    int min = (seconds % 3600) / 60;
    int hours = seconds / 3600;

    std::ostringstream stream;
    stream << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0') << min << ":"<< std::setw(2) << std::setfill('0') << sec;
     
     return stream.str();
}