#ifndef TAGUIBACKEND_H
#define TAGUIBACKEND_H

#include <QObject>
#include <QQmlEngine>

class TagUIBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TagUIBackend(QObject *parent = nullptr);
    Q_INVOKABLE QString qGetTrackName();
signals:
};

#endif // TAGUIBACKEND_H
