#ifndef LIBRARYDB_HPP
#define LIBRARYDB_HPP
#include <string>
#include "sqlite3.h"

constexpr const char *libFolderPath = "/VekAmp Data";
constexpr const char *libFileName = "/Library.db";

constexpr const char *initQuery =
    #include "SQL/initdb.sql"
;

class LibraryDB
{   
    public:
        static void InitLibrary();
        static void InitDatabase(std::string dirPath);
        static void CloseDatabase();
    private:
        static sqlite3* database;
        static void ProcessError(int err, const char* context = "No context.");

};

#endif // LIBRARYDB_HPP
