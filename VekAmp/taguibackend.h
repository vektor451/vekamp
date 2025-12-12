#ifndef TAGUIBACKEND_H
#define TAGUIBACKEND_H

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
    Q_INVOKABLE QString qGetTrackDetailStr();
    Q_INVOKABLE void qUpdateAlbumCover();

private:
    TagLib::FileRef GetCurTrackFileRef();

signals:
    void updateImage();
};

#endif // TAGUIBACKEND_H
