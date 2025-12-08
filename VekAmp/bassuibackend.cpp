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

    if (BASS::BASSPlayer::StartFilePlayback(nativePath.toStdString().c_str()))
    {
        BASS::BASSPlayer::StartPausePlayback();
    }
}

void BASSUIBackend::qMultifileSelect(QStringList fPaths)
{
    std::vector<std::string> queueVec;
    for(const auto& file : fPaths)
    {
        QUrl fPathUrl (file);
        queueVec.push_back(fPathUrl.toLocalFile().toUtf8().toStdString());
    }
    BASS::BASSPlayer::InitTrackQueue(queueVec);
}

void BASSUIBackend::qPlayPause()
{
    BASS::BASSPlayer::StartPausePlayback();
}

void BASSUIBackend::qNextTrack()
{
    BASS::BASSPlayer::GoNextTrack();
    BASS::BASSPlayer::StartPausePlayback();
}

void BASSUIBackend::qPrevHomeTrack()
{

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

void BASSUIBackend::EmitErrorMessage(QString message)
{
    emit bassError(message);
}

