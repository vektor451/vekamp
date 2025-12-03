#pragma once

#include <iostream>
#include <string>
#include <string_view>

#define nameof(name) std::string(#name)

std::string UlongToHex( unsigned long i );
bool StrEndsWith(std::string_view str, std::string_view suffix);