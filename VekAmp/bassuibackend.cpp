#include "bassuibackend.hpp"
#include "bassplayer.hpp"

BASSUIBackend::BASSUIBackend(QObject *parent)
    : QObject{parent}
{
    // HACK: assigning like this is a bit hacky, but it works.
    if(BASS::BASSPlayer::backendQObj)
    {
        qDebug("Multiple BASSUIBackends created: BASSPlayer already has a backendQObj set. Overriding existing backendQObj.");
    }
    BASS::BASSPlayer::backendQObj = this;
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
    std::string TrackProgress = BASS::BASSPlayer::GetTrackProgressStr(BASS::BASSPlayer::GetTrackProgressSecs());
    return QString::fromStdString(TrackProgress);
}

qreal BASSUIBackend::qGetTrackLen()
{
    double trackProgress = BASS::BASSPlayer::GetTrackProgressSecs();
    double trackLen = BASS::BASSPlayer::GetTrackLenSecs();
    return trackProgress / trackLen;
}

void BASSUIBackend::qSetTrackProgress(qreal value)
{
    if(!BASS::BASSPlayer::IsScrolling())
    {
        BASS::BASSPlayer::SetPos(value * BASS::BASSPlayer::GetTrackLenSecs());
    }
}

void BASSUIBackend::qSliderAdjustPause(bool pressed)
{
    if(pressed)
        BASS::BASSPlayer::StartScroll();
    else
        BASS::BASSPlayer::EndScroll();
}

void BASSUIBackend::qSetVolume(qreal value)
{
    BASS::BASSPlayer::SetVolume(value);
}

qreal BASSUIBackend::qGetVolume()
{
    return BASS::BASSPlayer::GetVolume();
}

void BASSUIBackend::EmitTrackChange()
{
    emit trackChanged();
}

