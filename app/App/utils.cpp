// c++
#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <sstream>

// self
#include "utils.hpp"

std::string UlongToHex(unsigned long i)
{
	std::stringstream stream;
	stream << "0x" 
			<< std::setfill ('0') << std::setw(sizeof(unsigned long)*2) 
			<< std::hex << i;
	return stream.str();
}

bool StrEndsWith(std::string_view str, std::string_view suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size()-suffix.size(), suffix.size(), suffix) == 0;
}