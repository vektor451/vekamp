#include "libraryuibackend.hpp"
#include "librarydb.hpp"
#include <QStandardPaths>
#include <thread>

LibraryUIBackend::LibraryUIBackend(QObject *parent)
    : QObject{parent}
{}

void LibraryUIBackend::qReindex()
{
    std::thread task(
        LibraryDB::BeginIndex,
        QStandardPaths::standardLocations(QStandardPaths::StandardLocation::MusicLocation).constFirst().toStdString()
    );
    task.detach();
}
