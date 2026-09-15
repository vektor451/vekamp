R"///("
-- VekAmp DB ver1.0 -- 

-- If you want to add new fields or constraints to existing tables 
-- create them at the end of the file and label the new version.

CREATE TABLE IF NOT EXISTS Artists (
	ArtistID INTEGER PRIMARY KEY,
	ArtistName TEXT NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS AlbumArtists (
	AlbumArtistID INTEGER PRIMARY KEY,
	AlbumArtistName TEXT NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS Genres (
	GenreID INTEGER PRIMARY KEY,
	GenreName TEXT NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS CoverArts (
	CoverArtID INTEGER PRIMARY KEY,
	CoverArtFilePath TEXT UNIQUE NOT NULL,
	CoverArtModTime INTEGER
);

CREATE TABLE IF NOT EXISTS Albums (
	AlbumID INTEGER PRIMARY KEY,
	AlbumName TEXT NOT NULL,
	AlbumYear INTEGER,
	
	AlbumArtistID INTEGER,
	CoverArtID INTEGER,
	
	FOREIGN KEY (AlbumArtistID)
		REFERENCES AlbumArtists (AlbumArtistID)
			ON DELETE RESTRICT
			ON UPDATE RESTRICT,
			
	FOREIGN KEY (CoverArtID)
		REFERENCES CoverArts (CoverArtID)
			ON DELETE SET NULL
			ON UPDATE NO ACTION,

        UNIQUE (AlbumName, AlbumArtistID)
);

CREATE TABLE IF NOT EXISTS Tracks (
	TrackID INTEGER PRIMARY KEY,
	TrackFilePath TEXT UNIQUE NOT NULL,
	TrackFileModTime INTEGER,
	TrackName TEXT,
	TrackLengthSecs INTEGER,
	TrackYear INTEGER,
	
	ArtistID INTEGER,
	CoverArtID INTEGER,
	AlbumID INTEGER,
	GenreID INTEGER,
	
	FOREIGN KEY (ArtistID)
		REFERENCES Artists (ArtistID)
			ON DELETE RESTRICT
			ON UPDATE RESTRICT,
			
	FOREIGN KEY (CoverArtID)
		REFERENCES CoverArts (CoverArtID)
			ON DELETE SET NULL
			ON UPDATE NO ACTION,
			
	FOREIGN KEY (AlbumID)
		REFERENCES Albums (AlbumID)
			ON DELETE RESTRICT
			ON UPDATE NO ACTION,
			
		FOREIGN KEY (GenreID)
		REFERENCES Genres (GenreID)
			ON DELETE RESTRICT
			ON UPDATE RESTRICT
);

-- End Of File )///"
