#include <iomanip>
#include <iostream>
#include "Commands.h"	
#include "Utilities.h"
#include "TypeDefs.h"
#include <sstream>
#include <ctime>

String convertTime(time_t time)
{
    std::tm unconvertedTime;
    localtime_s(&unconvertedTime, &time);

    std::ostringstream oss;
    oss << std::put_time(&unconvertedTime, "%m/%d/%Y, %I:%M:%S %p");

    return oss.str();
}