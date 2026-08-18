#ifndef TAGUIBACKEND_HPP
#define TAGUIBACKEND_HPP

#include <QObject>
#include <QQmlEngine>
#include "fileref.h"

class TagUIBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TagUIBackend(QObject *parent = nullptr);
    Q_INVOKABLE QString qGetCurTrackName();
    Q_INVOKABLE QString qGetTrackTitle(const char *filePath);
    Q_INVOKABLE QString qGetTrackArtist(const char *filePath, bool returnUnknown = false);
    Q_INVOKABLE QString qGetTrackLength(const char *filePath);
    Q_INVOKABLE QString qGetTrackDetailStr();
    Q_INVOKABLE void qUpdateAlbumCover();

private:
    TagLib::FileRef GetTrackFileRef(const char *filePath);

signals:
    void updateImage();
};

#endif // TAGUIBACKEND_H
