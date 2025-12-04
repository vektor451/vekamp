#include "utils.hpp"
#include "bassplayer.hpp"

#include <string>
#include <algorithm>
#include <string>
#include <string_view>
#include <cassert>

#include <QDebug>

namespace BASS
{
    std::string GetVersionStr()
    {
        std::string hexVer = UlongToHex(BASS_GetVersion());
        
        // example format = 0x02041100 = 2.4.11.
        std::string verNums[] = {
            hexVer.substr(2,2),
            hexVer.substr(4,2),
            hexVer.substr(6,2),
            hexVer.substr(8,2),
        };

        for(int i = 0; i < std::size(verNums); i++)
        {
            std::string str = verNums[i];
            
            if(str[0] == '0')
            {
                str = str.substr(1,1);
            }

            verNums[i] = str;
        }

        std::string verString = verNums[0] + '.' + verNums [1] + '.' + verNums[2] + '.' + verNums[3];

        return verString;
    }

    void BASSError(const char *text, bool isFatal)
    {
        qDebug("BASS Error(%d): %s\n", BASS_ErrorGetCode(), text);
        
        if(isFatal)
        {
            BASS_Free();
            exit(0);
        }
    }

    // AudioFormat Class.
    std::map<AudioFormat::StreamFormat, std::vector<std::string>> AudioFormat::ExtNames{
        {StreamFormat::WAV, 	{"WAV", "WAVE"}},
        {StreamFormat::FLAC, 	{"FLAC", }},
        {StreamFormat::ALAC, 	{"CAF"}},
        {StreamFormat::AIFF, 	{"AIFF", "AIF", "AIFC"}},
        {StreamFormat::AAC, 	{"AIFF", "AIF", "AIFC"}},
        {StreamFormat::APE, 	{"APE"}},
        {StreamFormat::AAC, 	{"AAC", "3GP", "ADIF", "ADTS"}},
        {StreamFormat::M4A, 	{"M4A", "M4R", "MP4", "M4P", "M4B", "M4V"}}, // BASS can play the audio from MP4s so why not.
        {StreamFormat::MP3, 	{"MP3", "MPGA"}},
        {StreamFormat::MP2, 	{"MP2", "MP2A", "M2A", "MPA"}},
        {StreamFormat::MP1, 	{"MP1"}},
        {StreamFormat::OGG, 	{"OGG"}},
        {StreamFormat::OPUS, 	{"OPUS"}},
    };
    
    AudioFormat::StreamFormat AudioFormat::GetFormat(std::string fPath)
    {
        std::transform(fPath.begin(), fPath.end(), fPath.begin(), std::toupper);
        for(int i = 0; i < StreamFormat::Count; i++)
        {
            size_t size = ExtNames[(StreamFormat)i].size();
            for(int f = 0; f < size; f++)
            {
                if(StrEndsWith(fPath, ExtNames[(StreamFormat)i][f]))
                {
                    return (StreamFormat)i;
                }
            }
        }

        return StreamFormat::NullFormat;
    }

    // BASSPlayer Class.
    float   	BASSPlayer::volume 			= 1.0;
    int     	BASSPlayer::deviceIdx 		= -1; // -1 = Default Device.
    DWORD   	BASSPlayer::curChannel 		= -1;
    QWORD   	BASSPlayer::trackLen 		= -1;
    std::string BASSPlayer::trackLenStr 	= "0:00";
    std::string BASSPlayer::curFilePath 	= "";
    bool    	BASSPlayer::restartChannel 	= FALSE;
    bool    	BASSPlayer::isPlaying 		= FALSE;

    void BASSPlayer::Init()
    {
        //Print out version.
        qDebug("Using BASS Version %s", BASS::GetVersionStr().c_str());
        
        // Check the correct BASS version was loaded.
        if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
            qDebug("An incorrect version of BASS was loaded.");
            exit(0);
        }

        // Attempts initialisation on default device. 
        if(!BASS_Init(deviceIdx, 48000, 0, 0, NULL))
            BASSError("Couldn't init device. (Incorrect deviceIdx?)");

        BASSPlayer::SetVolume(0.4);
    }

    void BASSPlayer::Destroy()
    {
        if(!BASS_Free())
            BASSError("Couldn't free BASS. (BASS not initialised?)", false);
        else
            qDebug("Bass Freed\n");
    }

    void BASSPlayer::StartFilePlayback(const char fPath[])
    {
        if(!BASS_ChannelStop(curChannel))
            BASSError("Couldn't stop channel. (No channel set?)", false);
        
        if(!BASS_ChannelFree(curChannel))
            BASSError("Couldn't free channel. (No channel set?)", false);
        
        AudioFormat::StreamFormat format = AudioFormat::GetFormat(fPath);

        DWORD StreamFlags = BASS_SAMPLE_FLOAT | BASS_STREAM_PRESCAN;
        
        // These formats should be definite.
        if(format == AudioFormat::FLAC)
            curChannel = BASS_FLAC_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);
        else if(format == AudioFormat::ALAC)
            curChannel = BASS_ALAC_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);
        else if(format == AudioFormat::APE)
            curChannel = BASS_APE_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);
        else if(format == AudioFormat::OPUS)
            curChannel = BASS_OPUS_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);
        // HACK: These are containers and might have multiple formats. We are going to brute force them.
        else if(format == AudioFormat::OGG) 
        {
            // Try as vorbis
            curChannel = BASS_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);
            
            // If func returns false, then file is not valid vorbis. Try as OPUS.
            if(!BASS_ChannelSetAttribute(curChannel, BASS_ATTRIB_VOL, volume))
                curChannel = BASS_OPUS_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);
        }
        else if(format == AudioFormat::M4A)
        {
            // Try as generic.
            curChannel = BASS_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);

            // If funcs return false, format is invalid. ALAC, FLAC, and OPUS are tried. 
            if(!BASS_ChannelSetAttribute(curChannel, BASS_ATTRIB_VOL, volume))
                curChannel = BASS_ALAC_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);

            if(!BASS_ChannelSetAttribute(curChannel, BASS_ATTRIB_VOL, volume))
                curChannel = BASS_FLAC_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);
            
            if(!BASS_ChannelSetAttribute(curChannel, BASS_ATTRIB_VOL, volume))
                curChannel = BASS_OPUS_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);
        }
        // Can be played by default in BASS, or just isn't a valid file.
        else
            curChannel = BASS_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);
        
        BASS_ChannelSetAttribute(curChannel, BASS_ATTRIB_VOL, volume);

        qDebug("Playing back path: %s", fPath);
        SetCurFilePath(fPath);

        trackLen = BASS_ChannelGetLength(curChannel, BASS_POS_BYTE);
        if(trackLen != -1)
        {
            double totalSecs = BASS_ChannelBytes2Seconds(curChannel, trackLen);
            int mins = (int)(totalSecs / 60.0);
            int secs = (int)totalSecs % 60;
            
			char lenStr[10];
            std::snprintf(lenStr, sizeof(lenStr), "%d:%02d", mins, secs);
			trackLenStr = lenStr;

            qDebug("Track length: %s (%f seconds)", trackLenStr.c_str(), totalSecs);
        } 
        else
        {
			BASSError("Could not get track length. (Invalid file?)", false);
			trackLenStr = "0:00";
		}

		isPlaying = false;
    }

    void BASSPlayer::StartPausePlayback()
    {
        // RESUME
        if (BASS_ChannelIsActive(curChannel) == BASS_ACTIVE_STOPPED
		|| BASS_ChannelIsActive(curChannel) == BASS_ACTIVE_PAUSED)
        {
            if(BASS_ChannelPlay(curChannel, FALSE))
            {
                qDebug("Playing Audio...");
                restartChannel = FALSE;
            }
            else
                BASS::BASSError("Couldn't play file. (Incorrect path?)", FALSE);

			isPlaying = true;
        }
        // PAUSE
        else if(BASS_ChannelIsActive(curChannel) == BASS_ACTIVE_PLAYING)
        {
            if(BASS_ChannelPause(curChannel))
            {
                qDebug("Pausing Playback.");
            }
            else
                BASS::BASSError("Couldn't pause. (No channel set?)", FALSE);

			isPlaying = false;
        }
    }

    void BASSPlayer::StopPlayback()
    {
        if(BASS_ChannelStop(curChannel))
        {
            qDebug("Stopping Playback.");
			BASS_ChannelSetPosition(curChannel, 0, BASS_POS_BYTE);
		}
        else
            BASS::BASSError("Couldn't stop. (No channel set?)", FALSE);

		isPlaying = false;
    }

	void BASSPlayer::StartScroll()
	{
		if(isPlaying)
		{
			if(BASS_ChannelPause(curChannel))
            {
                qDebug("Pausing Playback.");
            }
            else
                BASS::BASSError("Couldn't pause. (No channel set?)", FALSE);
		}
	}

	void BASSPlayer::EndScroll()
	{
		if(isPlaying)
		{
			if(BASS_ChannelPlay(curChannel, restartChannel))
            {
                qDebug("Playing Audio...");
                restartChannel = FALSE;
            }
            else
                BASS::BASSError("Couldn't play file. (Incorrect path?)", FALSE);
		}
	}

	void BASSPlayer::SetPos(double pos)
	{
		QWORD bytePos = BASS_ChannelSeconds2Bytes(curChannel, pos);
		BASS_ChannelSetPosition(curChannel, bytePos, BASS_POS_BYTE);
	}

	double BASSPlayer::GetTrackProgressSecs()	
	{
		return BASS_ChannelBytes2Seconds(
			curChannel, 
			BASS_ChannelGetPosition(curChannel, BASS_POS_BYTE)
		);
	}

	std::string BASSPlayer::GetTrackProgressStr(double pos)
	{
		const char *total = GetTrackLenStr();
		
		char posStr[10];
		char finalVal[25];
		
		if(pos != -1)
        {
            int mins = (int)(pos / 60.0);
            int secs = (int)pos % 60;
            
            std::snprintf(posStr, sizeof(posStr), "%d:%02d", mins, secs);
        } 
        else
        {
            std::snprintf(posStr, sizeof(posStr), "0:00");
		}

        std::snprintf(finalVal, 25, "%s / %s", posStr, total);
		std::string returnVal = finalVal;
		return returnVal;
	}

    // Setters & Getters
    void BASSPlayer::SetVolume(float vol)
    {
        volume = vol;
        qDebug("Set Volume to: %f", vol);

        BASS_ChannelSetAttribute(curChannel, BASS_ATTRIB_VOL, vol);
    }

    void BASSPlayer::SetCurFilePath(const char fPath[])
    {
        curFilePath = fPath;
    }

	// One liner Setters/Getters
    float       BASSPlayer::GetVolume() 		{return volume;}
	QWORD       BASSPlayer::GetTrackLen() 		{return trackLen;}
	double      BASSPlayer::GetTrackLenSecs()	{return BASS_ChannelBytes2Seconds(curChannel, trackLen);}
    const char *BASSPlayer::GetTrackLenStr() 	{return trackLenStr.c_str();}
    const char *BASSPlayer::GetCurFilePath() 	{return curFilePath.c_str();}
	bool        BASSPlayer::IsPlaying()			{return isPlaying;}

}
