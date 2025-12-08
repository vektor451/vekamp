#ifndef BASSUIBACKEND_HPP
#define BASSUIBACKEND_HPP

#include <QObject>
#include <QQmlEngine>
#include <QtTypes>

class BASSUIBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit BASSUIBackend(QObject *parent = nullptr);
    Q_INVOKABLE void qFileSelect(QUrl fPath);
    Q_INVOKABLE void qMultifileSelect(QStringList fPaths);

    Q_INVOKABLE void qPlayPause();
    Q_INVOKABLE void qNextTrack();
    Q_INVOKABLE void qPrevHomeTrack();

    Q_INVOKABLE QString qGetTrackLenStr();
    Q_INVOKABLE qreal qGetTrackLen();
    Q_INVOKABLE void qSetTrackProgress(qreal value);
    Q_INVOKABLE void qSliderAdjustPause(bool state);

    Q_INVOKABLE void qSetVolume(qreal value);
    Q_INVOKABLE qreal qGetVolume();

    void EmitTrackChange();
    void EmitErrorMessage(QString message);

signals:
    void trackChanged();
    void bassError(QString message);
};

#endif // BASSUIBACKEND_HPP
