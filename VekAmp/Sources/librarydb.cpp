#include "librarydb.hpp"
#include <QDebug>
#include <filesystem>

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
    if (database != nullptr)
    {
        sqlite3_close(database);
        database = nullptr;
    }

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

void LibraryDB::CloseDatabase()
{
    if (database != nullptr)
    {
        sqlite3_close(database);
        database = nullptr;
    }
}
