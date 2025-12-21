#include "miscuibackend.hpp"

MiscUIBackend::MiscUIBackend(QObject *parent)
    : QObject{parent}
{}

QString MiscUIBackend::qGetAboutPromptString()
{
    QString aboutText = VEKAMP_NAME;
    aboutText += " v";
    aboutText += VEKAMP_VER;
    aboutText += " // Built on: ";
    aboutText += BUILD_TIME;
    aboutText += "\nLicensed under GPLv3.";

    return aboutText;
}
