#ifndef COVERIMAGEPROVIDER_HPP
#define COVERIMAGEPROVIDER_HPP

#include <QQuickImageProvider>
#include <QPixmap>

class CoverImageProvider : public QQuickImageProvider
{
public:
    CoverImageProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
};

#endif // COVERIMAGEPROVIDER_HPP
