#pragma once

#include <iostream>
#include <string>
#include <string_view>

#define nameof(name) std::string(#name)

std::string UlongToHex( unsigned long i );

/// Function to return the filepath for a cover file based on files in directory of provided path.
/// Currently only checks for "cover" image.
std::wstring GetCoverFilePath( const char fPath[] );

bool StrEndsWith(std::string_view str, std::string_view suffix);
