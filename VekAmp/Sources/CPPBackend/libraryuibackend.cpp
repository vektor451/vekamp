#include "libraryuibackend.hpp"
#include "librarydb.hpp"
#include <QStandardPaths>
#include <thread>

std::vector<RecordCategoryEntry> LibraryUIBackend::recordCategoryEntries
    = std::vector<RecordCategoryEntry>();

LibraryUIBackend::LibraryUIBackend(QObject *parent)
    : QObject{parent}
{
    QObject::connect(
        LibraryDB::GetSingletonInstance(), &LibraryDB::refreshLibrary,
        this, &LibraryUIBackend::EmitRefreshLibrary
    );
}

void LibraryUIBackend::qReindex()
{
    std::thread task(
        LibraryDB::BeginIndex,
        QStandardPaths::standardLocations(QStandardPaths::StandardLocation::MusicLocation).constFirst().toStdString()
    );
    task.detach();
}

void LibraryUIBackend::RefreshRecords()
{
    recordCategoryEntries = LibraryDB::GetRecordCategoryEntries();
}

void LibraryUIBackend::EmitRefreshLibrary()
{
    RefreshRecords();
    emit refreshLibrary();
}

int LibraryUIBackend::qGetRecordCategoryCount()
{
    if(recordCategoryEntries.empty()) return -1;
    return recordCategoryEntries.size();
}

RecordCategoryEntry LibraryUIBackend::qGetRecordCategoryEntry(int idx)
{
    return recordCategoryEntries[idx];
}
