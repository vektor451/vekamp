#include "libraryuibackend.hpp"
#include "librarydb.hpp"
#include <QStandardPaths>
#include <thread>

auto LibraryUIBackend::recordCategoryEntries = std::vector<RecordCategoryEntry>();
auto LibraryUIBackend::recordEntries = std::vector<RecordEntry>();

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

void LibraryUIBackend::RefreshRecordCategories()
{
    recordCategoryEntries = LibraryDB::GetRecordCategoryEntries();
}

void LibraryUIBackend::qRefreshRecords(int selected)
{
    recordEntries = LibraryDB::GetRecordEntries(selected, this);
}

void LibraryUIBackend::EmitRefreshLibrary()
{
    RefreshRecordCategories();
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

int LibraryUIBackend::qGetRecordCount()
{
    if(recordEntries.empty()) return -1;
    return recordEntries.size();
}

RecordEntry LibraryUIBackend::qGetRecordEntry(int idx)
{
    return recordEntries[idx];
}

int RecordEntry::qGetTrackCount()
{
    if(mTracks.empty()) return -1;
    return mTracks.size();
}

TrackEntry RecordEntry::qGetTrackEntry(int idx)
{
    return mTracks[idx];
}
