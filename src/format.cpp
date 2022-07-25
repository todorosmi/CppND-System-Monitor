#include <string>


#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) { 
    int h, min, sec;
    if(seconds > 3600){
        h = (seconds - seconds%3600)/3600;
    }
    else { h = 0; }
    if(seconds - h*3600 > 60){
        min = (seconds-h*3600-(seconds-h*3600)%60)/60;
    }
    else {min = 0; }
    sec = seconds-h*3600-min*60;
    return std::to_string(h) + ':' + std::to_string(min) + ':' + std::to_string(sec);
}