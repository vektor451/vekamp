#ifndef BASSUIBACKEND_HPP
#define BASSUIBACKEND_HPP

#include <QObject>
#include <QQmlEngine>

class BASSUIBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit BASSUIBackend(QObject *parent = nullptr);
    Q_INVOKABLE void qClickMe();
    Q_INVOKABLE void qFileSelect(QUrl fPath);
    Q_INVOKABLE void qPlayPause();
    Q_INVOKABLE QString qGetTrackLenStr();
    Q_INVOKABLE qreal qGetTrackLen();

signals:
    void fileChanged(const char *fPath[]);
};

#endif // BASSUIBACKEND_HPP
