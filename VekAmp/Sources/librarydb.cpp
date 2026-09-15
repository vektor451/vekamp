#include "librarydb.hpp"
#include <QDebug>
#include <tag.h>
#include <fileref.h>
#include <tpropertymap.h>
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
    auto timeTakenM = std::chrono::duration_cast<std::chrono::minutes>(timeTaken);

    qDebug() << "Time taken to index:" << timeTakenM << timeTakenS << timeTakenMS;
}

// This is a recursive function.
void LibraryDB::IndexNode(std::filesystem::path dirPath)
{
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

            // TODO: check file hash to see if reindexing is necessary.

            // Artists
            long long artistID = -32768;
            TagLib::String artistString = file.tag()->artist();

            if(!artistString.isEmpty())
            {
                // insert artist
                const char *insertQuery = R"(
                    INSERT OR IGNORE INTO Artists (ArtistName)
                    VALUES (?)
                )";

                sqlite3_stmt *insertStatement = nullptr;

                ProcessError(sqlite3_prepare_v2(database, insertQuery, strlen(insertQuery), &insertStatement, &insertQuery));
                ProcessError(sqlite3_bind_text(insertStatement, 1, artistString.toCString(true), -1, SQLITE_STATIC));

                sqlite3_step(insertStatement);
                ProcessError(sqlite3_finalize(insertStatement));

                // get artist index
                const char *indexQuery = R"(
                    SELECT ArtistID FROM Artists WHERE ArtistName = ?
                )";

                sqlite3_stmt *indexStatement = nullptr;

                ProcessError(sqlite3_prepare_v2(database, indexQuery, strlen(indexQuery), &indexStatement, &indexQuery));
                ProcessError(sqlite3_bind_text(indexStatement, 1, artistString.toCString(true), -1, SQLITE_STATIC));

                ProcessError(sqlite3_step(indexStatement));
                artistID = sqlite3_column_int64(indexStatement, 0);
                ProcessError(sqlite3_step(indexStatement));

                ProcessError(sqlite3_finalize(indexStatement));
            }

            // AlbumArtists
            long long albumArtistID = -32768;
            TagLib::String albumArtistString = file.tag()->properties().value("ALBUMARTIST", {file.tag()->artist()})[0];

            if(!albumArtistString.isEmpty())
            {
                // insert
                const char *insertQuery = R"(
                    INSERT OR IGNORE INTO AlbumArtists (AlbumArtistName)
                    VALUES (?)
                )";

                sqlite3_stmt *insertStatement = nullptr;

                ProcessError(sqlite3_prepare_v2(database, insertQuery, strlen(insertQuery), &insertStatement, &insertQuery));
                ProcessError(sqlite3_bind_text(insertStatement, 1, albumArtistString.toCString(true), -1, SQLITE_STATIC));

                sqlite3_step(insertStatement);
                ProcessError(sqlite3_finalize(insertStatement));

                // get index
                const char *indexQuery = R"(
                    SELECT AlbumArtistID FROM AlbumArtists WHERE AlbumArtistName = ?
                )";

                sqlite3_stmt *indexStatement = nullptr;

                ProcessError(sqlite3_prepare_v2(database, indexQuery, strlen(indexQuery), &indexStatement, &indexQuery));
                ProcessError(sqlite3_bind_text(indexStatement, 1, albumArtistString.toCString(true), -1, SQLITE_STATIC));

                ProcessError(sqlite3_step(indexStatement));
                albumArtistID = sqlite3_column_int64(indexStatement, 0);
                ProcessError(sqlite3_step(indexStatement));

                ProcessError(sqlite3_finalize(indexStatement));
            }

            // Genres
            long long genreID = -32768;
            TagLib::String genreString = file.tag()->genre();

            if(!genreString.isEmpty())
            {
                const char *insertQuery = R"(
                    INSERT OR IGNORE INTO Genres (GenreName)
                    VALUES (?)
                )";

                sqlite3_stmt *insertStatement = nullptr;

                ProcessError(sqlite3_prepare_v2(database, insertQuery, strlen(insertQuery), &insertStatement, &insertQuery));
                ProcessError(sqlite3_bind_text(insertStatement, 1, genreString.toCString(true), -1, SQLITE_STATIC));

                sqlite3_step(insertStatement);
                ProcessError(sqlite3_finalize(insertStatement));

                // get index
                const char *indexQuery = R"(
                    SELECT GenreID FROM Genres WHERE GenreName = ?
                )";

                sqlite3_stmt *indexStatement = nullptr;

                ProcessError(sqlite3_prepare_v2(database, indexQuery, strlen(indexQuery), &indexStatement, &indexQuery));
                ProcessError(sqlite3_bind_text(indexStatement, 1, genreString.toCString(true), -1, SQLITE_STATIC));

                ProcessError(sqlite3_step(indexStatement));
                genreID = sqlite3_column_int64(indexStatement, 0);
                ProcessError(sqlite3_step(indexStatement));

                ProcessError(sqlite3_finalize(indexStatement));
            }

            // Cover Art
            long long coverArtID = -32768;
            std::u8string coverFilePath = GetCoverFilePathU8(entry.path().u8string());

            if(!coverFilePath.empty())
            {
                const char *insertQuery = R"(
                    INSERT OR IGNORE INTO CoverArts (CoverArtFilePath)
                    VALUES (?)
                )";

                const char *coverFilePathC = reinterpret_cast<const char *>(coverFilePath.c_str());

                sqlite3_stmt *insertStatement = nullptr;

                ProcessError(sqlite3_prepare_v2(database, insertQuery, strlen(insertQuery), &insertStatement, &insertQuery));
                ProcessError(sqlite3_bind_text(insertStatement, 1, coverFilePathC, -1, SQLITE_STATIC));

                sqlite3_step(insertStatement);
                ProcessError(sqlite3_finalize(insertStatement));

                // get index
                const char *indexQuery = R"(
                    SELECT CoverArtID, CoverArtModTime FROM CoverArts WHERE CoverArtFilePath = ?
                )";

                sqlite3_stmt *indexStatement = nullptr;

                ProcessError(sqlite3_prepare_v2(database, indexQuery, strlen(indexQuery), &indexStatement, &indexQuery));
                ProcessError(sqlite3_bind_text(indexStatement, 1, coverFilePathC, -1, SQLITE_STATIC));

                ProcessError(sqlite3_step(indexStatement));
                coverArtID = sqlite3_column_int64(indexStatement, 0);
                long long coverArtLastMod = sqlite3_column_int64(indexStatement, 1);
                ProcessError(sqlite3_step(indexStatement));

                ProcessError(sqlite3_finalize(indexStatement));

                long long timestamp = std::filesystem::directory_entry(coverFilePath).last_write_time().time_since_epoch().count();

                if (timestamp != coverArtLastMod)
                {
                    const char *updateQuery = R"(
                        UPDATE CoverArts SET CoverArtModTime = ? WHERE CoverArtID = ?
                    )";

                    sqlite3_stmt *updateStatement = nullptr;

                    ProcessError(sqlite3_prepare_v2(database, updateQuery, strlen(updateQuery), &updateStatement, &updateQuery));
                    ProcessError(sqlite3_bind_int(updateStatement, 1, timestamp));
                    ProcessError(sqlite3_bind_int(updateStatement, 2, coverArtID));

                    ProcessError(sqlite3_step(updateStatement));
                    ProcessError(sqlite3_finalize(updateStatement));
                }
            }

            // Albums
            long long albumID = -32768;
            TagLib::String albumString = file.tag()->album();
            unsigned int trackYear = file.tag()->year();

            if(!albumString.isEmpty())
            {
                const char *insertQuery = R"(
                    INSERT OR IGNORE INTO Albums (AlbumName, AlbumArtistID, CoverArtID)
                    VALUES (?, ?, ?)
                )";

                sqlite3_stmt *insertStatement = nullptr;

                ProcessError(sqlite3_prepare_v2(database, insertQuery, strlen(insertQuery), &insertStatement, &insertQuery));
                ProcessError(sqlite3_bind_text(insertStatement, 1, albumString.toCString(true), -1, SQLITE_STATIC));

                if(albumArtistID != -32768) ProcessError(sqlite3_bind_int64(insertStatement, 2, albumArtistID));
                if(coverArtID != -32768) ProcessError(sqlite3_bind_int64(insertStatement, 3, coverArtID));

                sqlite3_step(insertStatement);
                ProcessError(sqlite3_finalize(insertStatement));

                // get index
                const char *indexQuery = R"(
                    SELECT AlbumID, AlbumYear FROM Albums WHERE AlbumName = ?
                )";

                sqlite3_stmt *indexStatement = nullptr;

                ProcessError(sqlite3_prepare_v2(database, indexQuery, strlen(indexQuery), &indexStatement, &indexQuery));
                ProcessError(sqlite3_bind_text(indexStatement, 1, albumString.toCString(true), -1, SQLITE_STATIC));

                ProcessError(sqlite3_step(indexStatement));
                albumID = sqlite3_column_int64(indexStatement, 0);
                sqlite3_value *existYearValue = sqlite3_column_value(indexStatement, 1);
                ProcessError(sqlite3_step(indexStatement));

                ProcessError(sqlite3_finalize(indexStatement));

                bool updateYear = trackYear != 0;

                // insert album year if necessary
                if (sqlite3_value_type(existYearValue) == SQLITE_INTEGER && trackYear != 0)
                {
                    int existYear = sqlite3_value_int(existYearValue);
                    updateYear = trackYear < existYear;
                }

                if (updateYear)
                {
                    const char *updateQuery = R"(
                        UPDATE Albums SET AlbumYear = ? WHERE AlbumID = ?
                    )";

                    sqlite3_stmt *updateStatement = nullptr;

                    ProcessError(sqlite3_prepare_v2(database, updateQuery, strlen(updateQuery), &updateStatement, &updateQuery));
                    ProcessError(sqlite3_bind_int(updateStatement, 1, trackYear));
                    ProcessError(sqlite3_bind_int64(updateStatement, 2, albumID));

                    ProcessError(sqlite3_step(updateStatement));
                    ProcessError(sqlite3_finalize(updateStatement));
                }
            }

            // Tracks
            long long trackID;
            const char *insertQuery = R"(
                INSERT OR IGNORE INTO Tracks (TrackFilePath)
                VALUES (?)
            )";

            std::u8string u8filepath = entry.path().u8string();
            const char *filePathC = reinterpret_cast<const char *>(u8filepath.c_str());

            sqlite3_stmt *insertStatement = nullptr;

            ProcessError(sqlite3_prepare_v2(database, insertQuery, strlen(insertQuery), &insertStatement, &insertQuery));
            ProcessError(sqlite3_bind_text(insertStatement, 1, filePathC, -1, SQLITE_STATIC));

            sqlite3_step(insertStatement);
            ProcessError(sqlite3_finalize(insertStatement));

            // get index
            const char *indexQuery = R"(
                SELECT TrackID FROM Tracks WHERE TrackFilePath = ?
            )";

            sqlite3_stmt *indexStatement = nullptr;

            ProcessError(sqlite3_prepare_v2(database, indexQuery, strlen(indexQuery), &indexStatement, &indexQuery));
            ProcessError(sqlite3_bind_text(indexStatement, 1, filePathC, -1, SQLITE_STATIC));

            ProcessError(sqlite3_step(indexStatement));
            trackID = sqlite3_column_int64(indexStatement, 0);
            ProcessError(sqlite3_step(indexStatement));

            ProcessError(sqlite3_finalize(indexStatement));

            // update info
            const char *updateQuery = R"(
                UPDATE Tracks SET TrackFileModTime = ?, TrackName = ?, TrackLengthSecs = ?, TrackYear = ?, ArtistID = ?, CoverArtID = ?, AlbumID = ?, GenreID = ?
                WHERE TrackID = ?
            )";

            sqlite3_stmt *updateStatement = nullptr;

            TagLib::String titleTag = file.tag()->title();
            std::u8string u8filename = entry.path().stem().u8string();

            const char *title;

            if(titleTag.isEmpty())
            {
                title = reinterpret_cast<const char*>(u8filename.c_str());
            }
            else
            {
                title = titleTag.toCString(true);
            }

            ProcessError(sqlite3_prepare_v2(database, updateQuery, strlen(updateQuery), &updateStatement, &updateQuery));
            ProcessError(sqlite3_bind_int64(updateStatement, 1, entry.last_write_time().time_since_epoch().count()));
            ProcessError(sqlite3_bind_text(updateStatement, 2, title, -1, SQLITE_STATIC));
            ProcessError(sqlite3_bind_int64(updateStatement, 3, file.audioProperties()->lengthInSeconds()));

            if (trackYear != 0)          ProcessError(sqlite3_bind_int64(updateStatement, 4, trackYear));
            if (artistID != -32768)      ProcessError(sqlite3_bind_int64(updateStatement, 5, artistID));
            if (coverArtID != -32768)    ProcessError(sqlite3_bind_int64(updateStatement, 6, coverArtID));
            if (albumID != -32768)       ProcessError(sqlite3_bind_int64(updateStatement, 7, albumID));
            if (genreID != -32768)       ProcessError(sqlite3_bind_int64(updateStatement, 8, genreID));

            ProcessError(sqlite3_bind_int64(updateStatement, 9, trackID));

            ProcessError(sqlite3_step(updateStatement));
            ProcessError(sqlite3_finalize(updateStatement));
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
