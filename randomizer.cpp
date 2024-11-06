#include <iomanip>
#include <iostream>
#include "Commands.h"	
#include "Utilities.h"
#include "TypeDefs.h"
#include <sstream>
#include <ctime>
#include <random>

int randomNumber(int min, int max)
{
	return min + (std::rand() % (max - min+1));
}

size_t randomMemSize(size_t min, size_t max)
{
    // Calculate the minimum and maximum powers of 2 within the range
    size_t minPower = std::ceil(std::log2(min));
    size_t maxPower = std::floor(std::log2(max));

    // If the range is invalid (min is greater than max, or max is 0), return 0
    if (minPower > maxPower || maxPower == (size_t)-1) { // Check for maxPower overflow when maxMemPerProcess is 0
        return 0;
    }


    // Generate a random power within the valid range
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(minPower, maxPower);
    size_t randomPower = distrib(gen);

    // Return the calculated power of 2
    return (size_t)std::pow(2, randomPower);
}