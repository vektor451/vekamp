#ifndef COVERIMAGEPROVIDER_HPP
#define COVERIMAGEPROVIDER_HPP

#include <QQuickImageProvider>
#include <QPixmap>
#include <QByteArray>

class CoverImageProvider : public QQuickImageProvider
{
public:
    CoverImageProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

    static QByteArray imgData;
    static QString coverFilePath;
};

#endif // COVERIMAGEPROVIDER_HPP
