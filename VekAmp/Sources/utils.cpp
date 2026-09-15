#include "utils.hpp"
#include "bassplayer.hpp"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <string_view>
#include <sstream>
#include <filesystem>
#include <QDebug>

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

bool StrEndsWith(std::u8string_view str, std::u8string_view suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size()-suffix.size(), suffix.size(), suffix) == 0;
}

std::wstring coverFileNames[] = {
    L"cover.jpeg",
    L"cover.jpg",
    L"cover.png",
    L"folder.jpeg",
    L"folder.jpg",
    L"folder.png",
};

std::u8string coverFileNamesU8[] = {
    u8"cover.jpeg",
    u8"cover.jpg",
    u8"cover.png",
    u8"folder.jpeg",
    u8"folder.jpg",
    u8"folder.png",
};

std::wstring GetCoverFilePath(const char fPath[])
{
#if _WIN32
        size_t fNameLength =  MultiByteToWideChar(CP_UTF8, 0, fPath, -1, NULL, 0);
        std::wstring fNameBufStr;
        fNameBufStr.resize(fNameLength);
        MultiByteToWideChar(CP_UTF8, 0, fPath, -1, fNameBufStr.data(), fNameLength);
        //const WCHAR *fNameBuf = fNameBufStr.c_str();
#else
        const char *fNameBuf = fPath;
#endif

#if _WIN32
    std::wstring fileDir = std::filesystem::path(fNameBufStr).remove_filename().wstring();
#else
    std::string fileDir = std::filesystem::path(fNameBuf).remove_filename();
#endif

    for (const auto& file : std::filesystem::directory_iterator(fileDir))
    {
        std::wstring lowerFileName = file.path().filename().wstring();
        std::transform(lowerFileName.begin(), lowerFileName.end(), lowerFileName.begin(), [](const char &c){ return std::tolower(c); });

        // Check if cover exists.
        if(std::count(std::begin(coverFileNames), std::end(coverFileNames), lowerFileName) > 0)
        {
            return file.path().wstring();
        }
    }

    // No file found.
    return L"";
}

std::wstring GetCoverFilePath(const wchar_t fPath[])
{
    std::wstring fileDir = std::filesystem::path(fPath).remove_filename().wstring();

    for (const auto& file : std::filesystem::directory_iterator(fileDir))
    {
        std::wstring lowerFileName = file.path().filename().wstring();
        std::transform(lowerFileName.begin(), lowerFileName.end(), lowerFileName.begin(), [](const char &c){ return std::tolower(c); });

        // Check if cover exists.
        if(std::count(std::begin(coverFileNames), std::end(coverFileNames), lowerFileName) > 0)
        {
            return file.path().wstring();
        }
    }

    // No file found.
    return L"";
}

std::u8string GetCoverFilePathU8(std::u8string fPath)
{
    std::u8string fileDir = std::filesystem::path(fPath).remove_filename().u8string();

    for (const auto& file : std::filesystem::directory_iterator(fileDir))
    {
        std::u8string lowerFileName = file.path().filename().u8string();
        std::transform(lowerFileName.begin(), lowerFileName.end(), lowerFileName.begin(), [](const char8_t &c){ return std::tolower(c); });

        // Check if cover exists.
        if(std::count(std::begin(coverFileNamesU8), std::end(coverFileNamesU8), lowerFileName) > 0)
        {
            return file.path().u8string();
        }
    }

    // No file found.
    return std::u8string();
}

#if _WIN32

#include <fileapi.h>
const wchar_t *GetWinShortPathName(const wchar_t *fPath)
{
    long     length = 0;
    wchar_t* buffer = NULL;

    length = GetShortPathNameW(fPath, NULL, 0);

    buffer = new wchar_t[length];
    GetShortPathNameW(fPath, buffer, length);

    return buffer;
    // might leak memory lol
}
#endif
