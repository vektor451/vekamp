#pragma once

#include <iostream>
#include <string>
#include <string_view>

#define nameof(name) std::string(#name)

#if _WIN32
    using FileString = std::wstring;
#else
    using FileString = std::string;
#endif

std::string UlongToHex( unsigned long i );

/// Function to return the filepath for a cover file based on files in directory of provided path.
/// Currently only checks for "cover" image.
std::wstring GetCoverFilePath( const char fPath[] );

FileString StdStringToFileString(std::string string);
FileString StdWStringToFileString(std::wstring string);

bool StrEndsWith(std::string_view str, std::string_view suffix);
bool StrEndsWith(std::u8string_view str, std::u8string_view suffix);

#if _WIN32

const wchar_t *GetWinShortPathName(const wchar_t *fPath);

#endif
