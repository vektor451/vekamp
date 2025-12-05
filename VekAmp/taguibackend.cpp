#include "taguibackend.h"
#include "tag.h"
#include "fileref.h"
#include "bassplayer.hpp"

#include <string>
#include <filesystem>

TagUIBackend::TagUIBackend(QObject *parent)
    : QObject{parent}
{}

QString TagUIBackend::qGetCurTrackName()
{
    const char *curFilePath = BASS::BASSPlayer::GetCurFilePath();

#if _WIN32
    size_t fNameLength =  MultiByteToWideChar(CP_UTF8, 0, curFilePath, -1, NULL, 0);
    std::wstring fNameBufStr;
    fNameBufStr.resize(fNameLength);
    MultiByteToWideChar(CP_UTF8, 0, curFilePath, -1, fNameBufStr.data(), fNameLength);
    const WCHAR *fNameBuf = fNameBufStr.c_str();
#else
    const char *fNameBuf = fPath;
#endif

    TagLib::FileRef file(fNameBuf);
    TagLib::String title = file.tag()->title();
    TagLib::String artist = file.tag()->artist();

    QString finalString = "";

    if(!artist.isEmpty())
    {
        finalString += "**";
        finalString += artist.toCWString();
        finalString += "** - ";
    }

    if (title.isEmpty())
    {
        // Track name based on filename. This should be executed if there is no track metadata to go off.
        std::string fileString = std::filesystem::path(curFilePath).stem().string();
        finalString += fileString;
    }
    else
    {
        finalString += title.toCWString();
    }

    return finalString;
}
