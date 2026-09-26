#ifndef LIBRARYUIBACKEND_HPP
#define LIBRARYUIBACKEND_HPP

#include <QQmlEngine>

struct RecordCategoryEntry{
    Q_GADGET

    public:
        QString mCategoryName;
        QString mPicPath;
        QString mExtraInfo; // amt records, or artist.

        Q_PROPERTY(QString categoryName MEMBER mCategoryName);
        Q_PROPERTY(QString picPath MEMBER mPicPath);
        Q_PROPERTY(QString extraInfo MEMBER mExtraInfo);
};

class LibraryUIBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit LibraryUIBackend(QObject *parent = nullptr);
    Q_INVOKABLE void qReindex();

    // Record Category
    enum RecordCategoryType{
        AlbumArtist,
        Artist,
        Album,
        Genre,
    };

    Q_INVOKABLE int qGetRecordCategoryCount();
    Q_INVOKABLE RecordCategoryEntry qGetRecordCategoryEntry(int idx);

    public slots:
        void EmitRefreshLibrary();

    signals:
        void refreshLibrary();

    private:
        void RefreshRecords();
        static std::vector<RecordCategoryEntry> recordCategoryEntries;
};

#endif // LIBRARYUIBACKEND_HPP
