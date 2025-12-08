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

std::wstring coverFileNames[] = {
    L"cover.jpeg",
    L"cover.jpg",
    L"cover.png",
};

std::wstring GetCoverFilePath(const char fPath[])
{
    size_t fNameLength =  MultiByteToWideChar(CP_UTF8, 0, fPath, -1, NULL, 0);
    std::wstring fNameBufStr;
    fNameBufStr.resize(fNameLength);
    MultiByteToWideChar(CP_UTF8, 0, fPath, -1, fNameBufStr.data(), fNameLength);

    qDebug() << "begin";
    //auto foo = std::filesystem::path(fNameBufStr);
    qDebug() << "2";

    std::wstring fileDir = std::filesystem::path(fNameBufStr).remove_filename().wstring();

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


