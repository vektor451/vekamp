#include "bassuibackend.hpp"
#include "bassplayer.hpp"

BASSUIBackend::BASSUIBackend(QObject *parent)
    : QObject{parent}
{}

void BASSUIBackend::qClickMe()
{
    qDebug("Hello World! Clicky Click!");
}

void BASSUIBackend::qFileSelect(QUrl fPath)
{
    QString nativePath = fPath.toLocalFile().toUtf8();
    qDebug() << "File Selected: " << nativePath;
    BASS::BASSPlayer::StartFilePlayback(nativePath.toStdString().c_str());
    BASS::BASSPlayer::StartPausePlayback();
}

void BASSUIBackend::qPlayPause()
{
    BASS::BASSPlayer::StartPausePlayback();
}

QString BASSUIBackend::qGetTrackLenStr()
{
    return BASS::BASSPlayer::GetTrackLenStr();
}

qreal BASSUIBackend::qGetTrackLen()
{
    double trackProgress = BASS::BASSPlayer::GetTrackProgressSecs();
    double trackLen = BASS::BASSPlayer::GetTrackLenSecs();
    return trackProgress / trackLen;
}

void BASSUIBackend::qSetVolume(qreal value)
{
    BASS::BASSPlayer::SetVolume(value);
}

qreal BASSUIBackend::qGetVolume()
{
    return BASS::BASSPlayer::GetVolume();
}

