#include "taguibackend.h"
#include "tag.h"
#include "fileref.h"
#include "flacpicture.h"
#include "flacfile.h"
#include "mpegfile.h"
#include "id3v2.h"
#include "attachedpictureframe.h"
#include "id3v2tag.h"
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
    QString qcoverfPath = QString::fromStdWString(coverfPath);

    // Generic tag is good enough to see if we want a picture.
    if(file.complexPropertyKeys().contains("PICTURE") && qcoverfPath.isEmpty())
    {
        // Embedded cover fetching requires use of the specific tag objects.
        // MP3
        if(auto mpegFile = dynamic_cast<TagLib::MPEG::File *>(file.file()))
        {
            if(auto id3v2Tag = mpegFile->ID3v2Tag())
            {
                const TagLib::ID3v2::FrameList coverFrames = id3v2Tag->frameListMap()["APIC"];
                TagLib::ID3v2::AttachedPictureFrame *cover = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(coverFrames.front());
                CoverImageProvider::imgData = QByteArray(reinterpret_cast<const char*>(cover->picture().data()), cover->picture().size());
            }
        }
        // FLAC
        else if (auto flacFile = dynamic_cast<TagLib::FLAC::File *>(file.file()))
        {
            auto flacPictures = flacFile->pictureList();
            if(!flacPictures.isEmpty())
            {
                auto picData = flacPictures[0]->data();
                CoverImageProvider::imgData = QByteArray(reinterpret_cast<const char*>(picData.data()), picData.size());
            }
        }
    }
    else
    {
        CoverImageProvider::imgData.clear();
    }

    // Will attempt to get filepath of cover. If not found, it will return "".
    CoverImageProvider::coverFilePath = qcoverfPath;

    emit updateImage();
}


