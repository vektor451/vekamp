#include "bassuibackend.hpp"
#include "bassplayer.hpp"

BASSUIBackend::BASSUIBackend(QObject *parent)
    : QObject{parent}
{
    QObject::connect(
        BASS::BASSPlayer::GetSingletonInstance(), &BASS::BASSPlayer::trackChanged,
        this, &BASSUIBackend::EmitTrackChange
    );

    QObject::connect(
        BASS::BASSPlayer::GetSingletonInstance(), &BASS::BASSPlayer::bassError,
        this, &BASSUIBackend::EmitErrorMessage
    );

    QObject::connect(
        BASS::BASSPlayer::GetSingletonInstance(), &BASS::BASSPlayer::playStateChanged,
        this, &BASSUIBackend::EmitPlayStateChanged
    );

    QObject::connect(
        BASS::BASSPlayer::GetSingletonInstance(), &BASS::BASSPlayer::newTrackQueue,
        this, &BASSUIBackend::EmitNewTrackQueue
    );
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
}

void BASSUIBackend::qPrevHomeTrack()
{
    if(BASS::BASSPlayer::GetTrackProgressSecs() < 1.0)
    {
        BASS::BASSPlayer::GoPrevTrack();
    }
    else
    {
        BASS::BASSPlayer::SetPos(0);
    }
}

bool BASSUIBackend::qIsPlaying()
{
    return BASS::BASSPlayer::IsPlaying();
}

bool BASSUIBackend::qToggleShuffleMode()
{
    BASS::BASSPlayer::ToggleShuffleMode();
    return BASS::BASSPlayer::GetShuffleMode();
}

int BASSUIBackend::qToggleRepeatMode()
{
    BASS::BASSPlayer::ToggleRepeatMode();

    return (int)BASS::BASSPlayer::GetRepeatMode();
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

int BASSUIBackend::qGetTrackQueueLength()
{
    return BASS::BASSPlayer::GetTrackQueueLength();
}

QString BASSUIBackend::qGetTrackFileNameAtIndex(int idx)
{
    return BASS::BASSPlayer::GetTrackFileNameAtIndex(idx);
}

void BASSUIBackend::EmitTrackChange()
{
    emit trackChanged();
}

void BASSUIBackend::EmitNewTrackQueue()
{
    emit newTrackQueue();
}

void BASSUIBackend::EmitErrorMessage(QString message)
{
    emit bassError(message);
}

void BASSUIBackend::EmitPlayStateChanged()
{
    emit playStateChanged();
}

