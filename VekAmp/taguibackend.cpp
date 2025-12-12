#include "taguibackend.h"
#include "tag.h"
#include "fileref.h"
#include "flacpicture.h"
#include "flacfile.h"
#include "mpegfile.h"
#include "attachedpictureframe.h"
#include "id3v2tag.h"
#include "bassplayer.hpp"
#include "coverimageprovider.hpp"
#include "utils.hpp"

#include <string>
#include <filesystem>
#include <QDebug>
#include <format>

TagUIBackend::TagUIBackend(QObject *parent)
    : QObject{parent}
{}

QString TagUIBackend::qGetCurTrackName()
{
    const char *curFilePath = BASS::BASSPlayer::GetCurFilePath();

    TagLib::FileRef file = GetCurTrackFileRef();
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

QString TagUIBackend::qGetTrackDetailStr()
{
    const char *curFilePath = BASS::BASSPlayer::GetCurFilePath();

    TagLib::FileRef file = GetCurTrackFileRef();

    TagLib::String title = file.tag()->title();
    TagLib::String artist = file.tag()->artist();
    TagLib::String album = file.tag()->album();
    int year = file.tag()->year();

    int bitRate = file.audioProperties()->bitrate();
    int sampleRate = file.audioProperties()->sampleRate();
    int channels = file.audioProperties()->channels();

    QString outString = "<style type=\"text/css\">p { margin-top: 2px; margin-bottom: 2px; }</style>";

    if(!title.isEmpty())
    {
        outString += std::format("<p><b>{}</b>", title.toCString(true));
    }
    else
    {
        outString += std::format("<p>{}", std::filesystem::path(curFilePath).stem().string());
    }

    if(!artist.isEmpty())
    {
        outString += std::format("<p>{}", artist.toCString(true));
    }


    if(!artist.isEmpty())
    {
       outString += std::format("<p>{}", album.toCString(true));

        if(year != 0)
        {
            outString += std::format(" ({})", std::to_string(year));
        }

        outString += "";
    }

    std::string fileExt = std::filesystem::path(curFilePath).extension().string().erase(0, 1);
    std::transform(fileExt.begin(), fileExt.end(), fileExt.begin(), [](const char &c){ return std::toupper(c); });

    std::string channelString;
    if(channels == 1)
    {
        channelString = "Mono";
    }
    else if(channels == 2)
    {
        channelString = "Stereo";
    }
    else
    {
        channelString = std::format("{} channels", channels);
    }

    outString += std::format("<p><font color=grey><small>{}, {} kHz, {}k, {}, {}</font></small>",
                             fileExt, (double)sampleRate / 1000, bitRate, channelString, BASS::BASSPlayer::GetTrackLenStr());

    return outString;
}

// TODO: run this on a seperate thread.
void TagUIBackend::qUpdateAlbumCover()
{
    const char *curFilePath = BASS::BASSPlayer::GetCurFilePath();

    TagLib::FileRef file = GetCurTrackFileRef();

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

TagLib::FileRef TagUIBackend::GetCurTrackFileRef()
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
    return file;
}


