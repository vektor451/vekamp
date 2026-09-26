#ifndef LIBRARYUIBACKEND_HPP
#define LIBRARYUIBACKEND_HPP

#include <QQmlEngine>
#include <QQmlListProperty>

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

struct TrackEntry{
    Q_GADGET

public:
    QString mTrackName;
    //QString mPicPath;
    QString mYear;
    QString mArtist;
    QString mGenre;
    QString mLength;
    int mTrackNum;
    int mTrackDisc;

    Q_PROPERTY(QString trackName MEMBER mTrackName);
    Q_PROPERTY(QString year MEMBER mYear);
    Q_PROPERTY(QString artist MEMBER mArtist);
    Q_PROPERTY(QString genre MEMBER mGenre);
    Q_PROPERTY(QString length MEMBER mLength);
    Q_PROPERTY(int trackNum MEMBER mTrackNum);
    Q_PROPERTY(int trackDisc MEMBER mTrackDisc);
};

class RecordEntry{
    Q_GADGET

    public:
        QString mRecordName;
        QString mPicPath;
        QString mArtist;
        QString mYear;
        QString mGenre;
        QString mLength;

        std::vector<TrackEntry> mTracks;

        Q_PROPERTY(QString recordName MEMBER mRecordName);
        Q_PROPERTY(QString picPath MEMBER mPicPath);
        Q_PROPERTY(QString artist MEMBER mArtist);
        Q_PROPERTY(QString year MEMBER mYear);
        Q_PROPERTY(QString genre MEMBER mGenre);
        Q_PROPERTY(QString length MEMBER mLength);

        Q_INVOKABLE int qGetTrackCount();
        Q_INVOKABLE TrackEntry qGetTrackEntry(int idx);
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
    Q_INVOKABLE int qGetRecordCount();
    Q_INVOKABLE RecordEntry qGetRecordEntry(int idx);
    Q_INVOKABLE void qRefreshRecords(int selected);

    public slots:
        void EmitRefreshLibrary();

    signals:
        void refreshLibrary();

    private:
        void RefreshRecordCategories();
        static std::vector<RecordCategoryEntry> recordCategoryEntries;
        static std::vector<RecordEntry> recordEntries;
};

#endif // LIBRARYUIBACKEND_HPP
