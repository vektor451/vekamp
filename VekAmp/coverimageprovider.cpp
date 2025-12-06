#include "coverimageprovider.hpp"

CoverImageProvider::CoverImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

QPixmap CoverImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    int width = 256;
    int height = 256;

    if (size)
        *size = QSize(width, height);
    QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                   requestedSize.height() > 0 ? requestedSize.height() : height);

    pixmap.load("://Resources/emptycover.png");

    return pixmap.scaledToWidth(requestedSize.width(), Qt::TransformationMode::SmoothTransformation);
}
