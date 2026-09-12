#include "librarydb.hpp"
#include <QDebug>
#include <tag.h>
#include <fileref.h>
#include "bassplayer.hpp"
#include <chrono>
#include "utils.hpp"

sqlite3 * LibraryDB::database = nullptr;

void LibraryDB::InitLibrary()
{
    qDebug() << "Using SQLite version: " << sqlite3_libversion();
}

void LibraryDB::ProcessError(int err, const char* context)
{
    if (err == SQLITE_OK) return;
    if (err == SQLITE_DONE) return;
    if (err == SQLITE_ROW) return;

    qDebug() << "SQLite error: " << err << " (" << sqlite3_errstr(err) << ")"
             << " Context: " << context
             << "\n" << sqlite3_errmsg(database);
}

void LibraryDB::InitDatabase(std::string dirPath)
{
    CloseDatabase();

    std::filesystem::create_directory(dirPath + libFolderPath);
    std::filesystem::create_directory(dirPath + libFolderPath);

    ProcessError(
        sqlite3_open((dirPath + libFolderPath + libFileName).c_str(), &database),
        "init db open"
    );

    sqlite3_stmt *initDBStatement = nullptr;
    const char *initDBStatementStr = initQuery + 1; // ignore first char (")
    int err = SQLITE_OK;

    do{
        ProcessError(
            sqlite3_prepare_v2(database, initDBStatementStr, strlen(initDBStatementStr), &initDBStatement, &initDBStatementStr),
            "database init sql preperation"
        );

        err = sqlite3_step(initDBStatement);

        ProcessError(sqlite3_finalize(initDBStatement), "init statement");
    } while (err == SQLITE_DONE);
}

void LibraryDB::BeginIndex(std::string dirPath)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    auto startTimestamp = startTime.time_since_epoch();

    // TODO: Implement threading
    qDebug() << "Beginning index procedure.";
    IndexNode(dirPath);

    auto endTime = std::chrono::high_resolution_clock::now();
    auto endTimestamp = endTime.time_since_epoch();

    auto timeTaken = endTimestamp - startTimestamp;
    auto timeTakenMS = std::chrono::duration_cast<std::chrono::milliseconds>(timeTaken) % 1000;
    auto timeTakenS = std::chrono::duration_cast<std::chrono::seconds>(timeTaken) % 60;
    auto timeTakenM = std::chrono::duration_cast<std::chrono::minutes>(timeTaken) / (1000 * 60);

    qDebug() << "Time taken to index:" << timeTakenM << timeTakenS << timeTakenMS;
}

// This is a recursive function.
void LibraryDB::IndexNode(std::filesystem::path dirPath)
{
    qDebug() << u8"Indexing dir: " + dirPath.wstring();
    for (const auto& entry: std::filesystem::directory_iterator(dirPath))
    {
        if (entry.is_directory())
        {
            IndexNode(entry.path());
        }

        if(entry.is_regular_file())
        {
            if (BASS::AudioFormat::GetFormat(entry.path().u8string()) == BASS::AudioFormat::NullFormat) continue;
            qDebug() << u8"Indexing file: " + entry.path().u8string();

            TagLib::FileRef file(entry.path().c_str());

#if _WIN32
            // Paths over 260 chars long on NTFS systems might truncate path names to make things fit.
            // To make it work on these systems, we need convert the long path to a short path.
            // I hate NTFS.
            if (file.isNull() && entry.path().string().length() >= 260)
            {
                auto winShortName = GetWinShortPathName(entry.path().c_str());
                qDebug() << "Retrying with short path name: " << std::wstring(winShortName);
                file = TagLib::FileRef(winShortName);
            }
#endif

            if (file.isNull())
            {
                qDebug() << u8"File is invalid!: " + entry.path().u8string();
                continue;
            }

            // Database stuff.

            // Artists

            // AlbumArtists

            // Genres

            // Cover Art

            // Albums

            // Tracks
        }
    }
}

void LibraryDB::CloseDatabase()
{
    if (database != nullptr)
    {
        sqlite3_close(database);
        database = nullptr;
    }
}
