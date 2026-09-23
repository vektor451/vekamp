#include "coverimageprovider.hpp"
#include <QSaveFile>

CoverImageProvider::CoverImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

QByteArray CoverImageProvider::imgData;
QString CoverImageProvider::coverFilePath;

// TODO: Make asynchronous.
QPixmap CoverImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "Requesting new image...";

    int width = 1024;
    int height = 1024;

    if (size)
        *size = QSize(width, height);
    QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                   requestedSize.height() > 0 ? requestedSize.height() : height);

    if(!coverFilePath.isEmpty())
    {
        qDebug() << "Loading from sibling file.";
        pixmap.load(coverFilePath);
    }
    else if(imgData.isEmpty())
    {
        qDebug() << "No cover data to load.";
        pixmap.load("://Resources/emptycover.png");
    }
    else
    {
        // FIXME: Make sure embedded data is loaded with correct asepct ratio.
        qDebug() << "Loading from embedded data.";
        pixmap.loadFromData(imgData);
    }

    return pixmap.scaledToHeight(requestedSize.height(), Qt::TransformationMode::SmoothTransformation);
}
