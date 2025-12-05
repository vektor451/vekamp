#include "taguibackend.h"
#include "tag.h"
#include "fileref.h"
#include "bassplayer.hpp"

#include <string>
#include <filesystem>

TagUIBackend::TagUIBackend(QObject *parent)
    : QObject{parent}
{}

QString TagUIBackend::qGetTrackName()
{
    std::string curFilePath = BASS::BASSPlayer::GetCurFilePath();

    TagLib::FileRef file(curFilePath.c_str());
    TagLib::String title = file.tag()->title();

    if (title.isEmpty())
    {
        // Track name based on filename. This should be executed if there is no metadata to go off.
        std::string fileString = std::filesystem::path(curFilePath).stem().string();
        return QString::fromStdString(fileString);
    }
    else
    {
        return QString::fromWCharArray(title.toCWString());
    }
}
