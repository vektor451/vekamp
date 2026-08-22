#ifndef MISCUIBACKEND_HPP
#define MISCUIBACKEND_HPP

#include <QObject>
#include <QQmlEngine>

class MiscUIBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit MiscUIBackend(QObject *parent = nullptr);
    Q_INVOKABLE QString qGetAboutPromptString();
signals:
};

#endif // MISCUIBACKEND_HPP
