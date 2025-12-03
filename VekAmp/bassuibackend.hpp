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

signals:
};

#endif // BASSUIBACKEND_HPP
