#include "taguibackend.h"
#include "tag.h"
#include "fileref.h"
#include "flacpicture.h"
#include "id3v2.h"
#include "bassplayer.hpp"
#include "coverimageprovider.hpp"
#include "utils.hpp"

#include <string>
#include <filesystem>
#include <fstream>
#include <QDebug>

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

// TODO: run this on a seperate thread.
void TagUIBackend::qUpdateAlbumCover()
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

    std::wstring coverfPath = GetCoverFilePath(curFilePath);

    // Generic tag is good enough to see if we want a picture.
    if(file.complexPropertyKeys().contains("PICTURE") && !coverfPath.empty())
    {
        // Embedded cover fetching requires use of the specific tag objects.

    }
    else
    {
        CoverImageProvider::imgData.clear();
    }

    // Will attempt to get filepath of cover. If not found, it will return "".
    CoverImageProvider::coverFilePath = QString::fromStdWString(coverfPath);
    emit updateImage();
}


