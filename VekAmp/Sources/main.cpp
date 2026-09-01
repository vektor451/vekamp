#include "bassplayer.hpp"
#include "librarydb.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QCommandLineParser>
#include <QPalette>
#include <QQuickImageProvider>
#include <coverimageprovider.hpp>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    // Initialise BASSPlayer Singleton
    BASS::BASSPlayer singleton = BASS::BASSPlayer();

    QGuiApplication app(argc, argv);
    app.setApplicationName("VekAmp");
    app.setApplicationVersion(VEKAMP_VER);

    QCommandLineParser parser;
    parser.setApplicationDescription("VekAmp Music Player.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "File to open.");

    parser.process(app);
    const QStringList args = parser.positionalArguments();

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.addImageProvider("coverImage", new CoverImageProvider);
    engine.loadFromModule("VekAmp", "Main");

    // Init RNG;
    srand(time(NULL));

    // Bass Init
    BASS::BASSPlayer::Init();

    // SQLite Init
    LibraryDB::InitLibrary();
    LibraryDB::InitDatabase(QStandardPaths::standardLocations(QStandardPaths::StandardLocation::MusicLocation).constFirst().toStdString());

    if(args.count())
    {
        BASS::BASSPlayer::StartFilePlayback(args.at(0).toStdString().c_str());
        BASS::BASSPlayer::StartPausePlayback();
    }

    int exit = app.exec();
    BASS::BASSPlayer::Destroy();
    LibraryDB::CloseDatabase();
    return exit;
}
