#include "librarydb.hpp"
#include "sqlite3.h"
#include <QDebug>

void LibraryDB::InitLibrary()
{
    qDebug() << "Using SQLite version: " << sqlite3_libversion();
}
