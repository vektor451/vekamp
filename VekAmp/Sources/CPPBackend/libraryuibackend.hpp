#ifndef LIBRARYUIBACKEND_HPP
#define LIBRARYUIBACKEND_HPP

#include <QQmlEngine>

class LibraryUIBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit LibraryUIBackend(QObject *parent = nullptr);
    Q_INVOKABLE void qReindex();

signals:
};

#endif // LIBRARYUIBACKEND_HPP
