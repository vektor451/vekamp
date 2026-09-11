#include "libraryuibackend.hpp"
#include "librarydb.hpp"
#include <QStandardPaths>

LibraryUIBackend::LibraryUIBackend(QObject *parent)
    : QObject{parent}
{}

void LibraryUIBackend::qReindex()
{
    LibraryDB::BeginIndex(QStandardPaths::standardLocations(QStandardPaths::StandardLocation::MusicLocation).constFirst().toStdString());
}
