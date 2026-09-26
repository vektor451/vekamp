#ifndef LIBRARYDB_HPP
#define LIBRARYDB_HPP
#include <string>
#include "sqlite3.h"
#include <filesystem>
#include <QObject>
#include <QQmlEngine>
#include <QtTypes>
#include "libraryuibackend.hpp"


constexpr const char *libFolderPath = "/VekAmp Data";
constexpr const char *libFileName = "/Library.db";

constexpr const char *initQuery =
    #include "SQL/initdb.sql"
;

class LibraryDB : public QObject
{
    Q_OBJECT

    public:
        explicit LibraryDB(QObject *parent = nullptr);
        static LibraryDB *GetSingletonInstance();
        static void InitLibrary();
        static void InitDatabase(std::string dirPath);
        static void BeginIndex(std::string dirPath);
        static void CloseDatabase();
        static std::vector<RecordCategoryEntry> GetRecordCategoryEntries();
        static std::vector<RecordEntry> GetRecordEntries(int categoryIdx, LibraryUIBackend *backendObj);
    private:
        static sqlite3 *database;
        static LibraryDB *singletonInstance;

        static void ProcessError(int err, const char* context = "No context.");
        static void IndexNode(std::filesystem::path dirPath);
    signals:
        void refreshLibrary();
};

#endif // LIBRARYDB_HPP
