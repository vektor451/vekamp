#include "bassuibackend.hpp"

BASSUIBackend::BASSUIBackend(QObject *parent)
    : QObject{parent}
{}

void BASSUIBackend::qClickMe()
{
    qDebug("Hello World! Clicky Click!");
}

