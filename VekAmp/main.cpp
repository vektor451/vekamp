#include "bassplayer.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
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
    engine.loadFromModule("VekAmp", "Main");

    // Bass Init
    BASS::BASSPlayer::Init();

    if(args.count())
    {
        BASS::BASSPlayer::StartFilePlayback(args.at(0).toStdString().c_str());
        BASS::BASSPlayer::StartPausePlayback();
    }

    return app.exec();
}
