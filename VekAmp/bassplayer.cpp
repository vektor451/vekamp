#include "utils.hpp"
#include "bassplayer.hpp"

#include <iterator>
#include <filesystem>
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

    std::string GetErrorStr(int err)
    {
        std::map<int, std::string> BASSErrorCodeStr{
            {BASS_OK,                   "BASS_OK"},
            {BASS_ERROR_MEM,            "BASS_ERROR_MEM"},
            {BASS_ERROR_FILEOPEN,       "BASS_ERROR_FILEOPEN"},
            {BASS_ERROR_DRIVER,         "BASS_ERROR_DRIVER"},
            {BASS_ERROR_BUFLOST,        "BASS_ERROR_BUFLOST"},
            {BASS_ERROR_HANDLE,         "BASS_ERROR_HANDLE"},
            {BASS_ERROR_FORMAT,         "BASS_ERROR_FORMAT"},
            {BASS_ERROR_POSITION,       "BASS_ERROR_POSITION"},
            {BASS_ERROR_INIT,           "BASS_ERROR_INIT"},
            {BASS_ERROR_START,          "BASS_ERROR_START"},
            {BASS_ERROR_SSL,            "BASS_ERROR_SSL"},
            {BASS_ERROR_REINIT,         "BASS_ERROR_REINIT"},
            {BASS_ERROR_ALREADY,        "BASS_ERROR_ALREADY"},
            {BASS_ERROR_NOTAUDIO,       "BASS_ERROR_NOTAUDIO"},
            {BASS_ERROR_NOCHAN,         "BASS_ERROR_NOCHAN"},
            {BASS_ERROR_ILLTYPE,        "BASS_ERROR_ILLTYPE"},
            {BASS_ERROR_ILLPARAM,       "BASS_ERROR_ILLPARAM"},
            {BASS_ERROR_NO3D,           "BASS_ERROR_NO3D"},
            {BASS_ERROR_NOEAX,          "BASS_ERROR_NOEAX"},
            {BASS_ERROR_DEVICE,         "BASS_ERROR_DEVICE"},
            {BASS_ERROR_NOPLAY,         "BASS_ERROR_NOPLAY"},
            {BASS_ERROR_FREQ,           "BASS_ERROR_FREQ"},
            {BASS_ERROR_NOTFILE,        "BASS_ERROR_NOTFILE"},
            {BASS_ERROR_NOHW,           "BASS_ERROR_NOHW"},
            {BASS_ERROR_EMPTY,          "BASS_ERROR_EMPTY"},
            {BASS_ERROR_NONET,          "BASS_ERROR_NONET"},
            {BASS_ERROR_CREATE,         "BASS_ERROR_CREATE"},
            {BASS_ERROR_NOFX,           "BASS_ERROR_NOFX"},
            {BASS_ERROR_NOTAVAIL,       "BASS_ERROR_NOTAVAIL"},
            {BASS_ERROR_DECODE,         "BASS_ERROR_DECODE"},
            {BASS_ERROR_DX,             "BASS_ERROR_DX"},
            {BASS_ERROR_TIMEOUT,        "BASS_ERROR_TIMEOUT"},
            {BASS_ERROR_FILEFORM,       "BASS_ERROR_FILEFORM"},
            {BASS_ERROR_SPEAKER,        "BASS_ERROR_SPEAKER"},
            {BASS_ERROR_VERSION,        "BASS_ERROR_VERSION"},
            {BASS_ERROR_CODEC,          "BASS_ERROR_CODEC"},
            {BASS_ERROR_ENDED,          "BASS_ERROR_ENDED"},
            {BASS_ERROR_BUSY,           "BASS_ERROR_BUSY"},
            {BASS_ERROR_UNSTREAMABLE,   "BASS_ERROR_UNSTREAMABLE"},
            {BASS_ERROR_PROTOCOL,       "BASS_ERROR_PROTOCOL"},
            {BASS_ERROR_DENIED,         "BASS_ERROR_DENIED"},
            {BASS_ERROR_UNKNOWN,        "BASS_ERROR_UNKNOWN"},
        };

        return BASSErrorCodeStr[err];
    }

    void BASSError(const char *text, bool isFatal, bool showGUI)
    {
        qDebug("BASS Error: %s (%s)", GetErrorStr(BASS_ErrorGetCode()).c_str(), text);

        if(isFatal)
        {
            BASS_Free();
            exit(0);
        }

        if(showGUI && BASS::BASSPlayer::backendQObj)
        {
            QString str = "**A BASS Error has occured:** ";
            str += GetErrorStr(BASS_ErrorGetCode());
            str += "\n\n";
            str += text;
            BASS::BASSPlayer::backendQObj->EmitErrorMessage(str);
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
        std::transform(fPath.begin(), fPath.end(), fPath.begin(), [](const char &c){ return std::toupper(c); });
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
    DWORD       BASSPlayer::queuedChannel{};
    QWORD   	BASSPlayer::trackLen 		= -1;
    std::string BASSPlayer::trackLenStr 	= "0:00";
    std::string BASSPlayer::curFilePath 	= "";
    bool    	BASSPlayer::restartChannel 	= FALSE;
    bool    	BASSPlayer::isPlaying 		= FALSE;
    bool    	BASSPlayer::isScrolling 	= TRUE;
    int         BASSPlayer::trackQueueIdx = 0;

    std::vector<std::string> BASSPlayer::trackQueue{};


    BASSUIBackend * BASSPlayer::backendQObj = nullptr;

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
        if(queueThread.joinable())
            queueThread.join();

        if(!BASS_Free())
            BASSError("Couldn't free BASS. (BASS not initialised?)", false);
        else
            qDebug("Bass Freed\n");
    }

    bool BASSPlayer::CreateChannelStream(const char fPath[], DWORD *channel)
    {
        qDebug() << "Creating stream for: " << fPath;

        AudioFormat::StreamFormat format = AudioFormat::GetFormat(fPath);

        DWORD StreamFlags = BASS_SAMPLE_FLOAT | BASS_STREAM_PRESCAN;
#if _WIN32
        size_t fNameLength =  MultiByteToWideChar(CP_UTF8, 0, fPath, -1, NULL, 0);
        std::wstring fNameBufStr;
        fNameBufStr.resize(fNameLength);
        MultiByteToWideChar(CP_UTF8, 0, fPath, -1, fNameBufStr.data(), fNameLength);
        const WCHAR *fNameBuf = fNameBufStr.c_str();
#else
        const char *fNameBuf = fPath;
#endif
        // These formats should be definite.
        if(format == AudioFormat::FLAC)
            *channel = BASS_FLAC_StreamCreateFile(FALSE, fNameBuf, 0, 0, StreamFlags);
        else if(format == AudioFormat::ALAC)
            *channel = BASS_ALAC_StreamCreateFile(FALSE, fNameBuf, 0, 0, StreamFlags);
        else if(format == AudioFormat::APE)
            *channel = BASS_APE_StreamCreateFile(FALSE, fNameBuf, 0, 0, StreamFlags);
        else if(format == AudioFormat::OPUS)
            *channel = BASS_OPUS_StreamCreateFile(FALSE, fNameBuf, 0, 0, StreamFlags);
        // HACK: These are containers and might have multiple formats. We are going to brute force them.
        else if(format == AudioFormat::OGG)
        {
            // Try as vorbis
            *channel = BASS_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);

            // If file format invalid, try as OPUS.
            if(BASS_ErrorGetCode() != BASS_OK)
                *channel = BASS_OPUS_StreamCreateFile(FALSE, fNameBuf, 0, 0, StreamFlags);
        }
        else if(format == AudioFormat::M4A)
        {
            // Generic BASS has FLAC support but it's unstable. Try flac before generic.
            *channel = BASS_FLAC_StreamCreateFile(FALSE, fNameBuf, 0, 0, StreamFlags);

            // If not FLAC, try ALAC, OPUS, and Generic
            if(BASS_ErrorGetCode() != BASS_OK)
                *channel = BASS_ALAC_StreamCreateFile(FALSE, fNameBuf, 0, 0, StreamFlags);

            if(BASS_ErrorGetCode() != BASS_OK)
                *channel = BASS_OPUS_StreamCreateFile(FALSE, fNameBuf, 0, 0, StreamFlags);

            if(BASS_ErrorGetCode() != BASS_OK)
                *channel = BASS_StreamCreateFile(FALSE, fPath, 0, 0, StreamFlags);
        }
        // Can be played by default in BASS, or else it just isn't a valid file.
        else
            *channel = BASS_StreamCreateFile(FALSE, fNameBuf, 0, 0, StreamFlags);

        BASS_ChannelSetSync(*channel, BASS_SYNC_END, 0, TrackFinishedProcess, 0);

        return BASS_ErrorGetCode() == BASS_OK;
    }

    bool BASSPlayer::StartFilePlayback(const char fPath[], bool queued)
    {
        if(!queued)
        {
            if(!BASS_ChannelStop(curChannel))
                BASSError("Couldn't stop channel. (No channel set?)", false);

            if(!BASS_ChannelFree(curChannel))
                BASSError("Couldn't free channel. (No channel set?)", false);

            BASSPlayer::CreateChannelStream(fPath, &curChannel);
            qDebug() << "Current Channel: " << curChannel;
        }
        else
        {
            if(queueThread.joinable())
            {
                queueThread.join();

                qDebug() << "Queued Channel: " << queuedChannel;

                if(!BASS_ChannelStop(curChannel))
                    BASSError("Couldn't stop channel. (No channel set?)", false);

                if(!BASS_ChannelFree(curChannel))
                    BASSError("Couldn't free channel. (No channel set?)", false);

                curChannel = queuedChannel;
            }
            else
            {
                return false;
            }
        }

        if(BASS_ErrorGetCode() != BASS_OK)
        {
            BASSError("File is invalid!", false, true);

            // possible recursion? if curFilePath is set to something invalid to begin with.
            if(curFilePath != "")
            {
                StartFilePlayback(GetCurFilePath());
            }

            return false;
        }
        
        BASS_ChannelSetAttribute(curChannel, BASS_ATTRIB_VOL, volume);

        qDebug("Playing back path: %s", fPath);
        SetCurFilePath(fPath);

        if(backendQObj)
        {
            backendQObj->EmitTrackChange();
        }
        else
        {
            qDebug("backendQObj is a nullptr! Cannot emit signals.");
        }

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

        if(!trackQueue.empty())
            QueueNextTrack();

        return true;
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
            {
                qDebug() << "Playback failure on channel" << curChannel;
                BASS::BASSError("Couldn't play file. (Incorrect path?)", FALSE);
            }

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
        isScrolling = true;

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
        if(isPlaying && isScrolling)
		{
			if(BASS_ChannelPlay(curChannel, restartChannel))
            {
                qDebug("Playing Audio...");
                restartChannel = FALSE;
            }
            else
                BASS::BASSError("Couldn't play file. (Incorrect path?)", FALSE);
		}

        isScrolling = false;
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

    void BASSPlayer::InitTrackQueue(std::vector<std::string> newQueue)
    {
        trackQueueIdx = 0;
        trackQueue = newQueue;
        qDebug() << "New Queue: " << trackQueue;

        //QueueNextTrack();
    }

    void BASSPlayer::GoNextTrack()
    {
        if(trackQueue.size() > 1)
        {
            trackQueueIdx = GetNextTrackQueueIdx();
            StartFilePlayback(trackQueue[trackQueueIdx].c_str(), true);
            StartPausePlayback();
        }
        else if(trackQueue.size() == 1)
        {
            SetPos(0);
        }

        qDebug() << "Current track queue idx" << trackQueueIdx;
    }

    void BASSPlayer::GoPrevTrack()
    {
        if(trackQueue.size() > 1)
        {
            trackQueueIdx = GetPrevTrackQueueIdx();
            StartFilePlayback(trackQueue[trackQueueIdx].c_str(), false);
            StartPausePlayback();
        }
        else if(trackQueue.size() == 1)
        {
            SetPos(0);
        }

        qDebug() << "Current track queue idx" << trackQueueIdx;
    }

    std::thread BASSPlayer::queueThread{};

    void BASSPlayer::QueueNextTrack()
    {
        if(queueThread.joinable())
            queueThread.join();

        if(trackQueue.size() > 1)
        {
            int queueIdx = GetNextTrackQueueIdx();

            std::thread queueWork(BASSPlayer::CreateChannelStream, trackQueue[queueIdx].c_str(), &queuedChannel);
            queueThread.swap(queueWork);
        }
    }

    void BASSPlayer::TrackFinishedProcess(HSYNC handle, DWORD channel, DWORD data, void *user)
    {
        GoNextTrack();
    }

    int BASSPlayer::GetNextTrackQueueIdx()
    {
        if(trackQueue.empty())
            return 0;

        if(trackQueueIdx + 1 >= trackQueue.size())
            return 0;
        else
            return trackQueueIdx + 1;
    }

    int BASSPlayer::GetPrevTrackQueueIdx()
    {
        if(trackQueue.empty())
            return 0;

        if(trackQueueIdx - 1 <= 0)
            return trackQueue.size() - 1;
        else
            return trackQueueIdx - 1;
    }

	// One liner Setters/Getters
    float       BASSPlayer::GetVolume() 		{return volume;}
	QWORD       BASSPlayer::GetTrackLen() 		{return trackLen;}
	double      BASSPlayer::GetTrackLenSecs()	{return BASS_ChannelBytes2Seconds(curChannel, trackLen);}
    const char *BASSPlayer::GetTrackLenStr() 	{return trackLenStr.c_str();}
    const char *BASSPlayer::GetCurFilePath() 	{return curFilePath.c_str();}
    bool        BASSPlayer::IsPlaying()			{return isPlaying;}
    bool        BASSPlayer::IsScrolling()       {return isScrolling;}
    int         BASSPlayer::GetTrackQueueIdx()  {return trackQueueIdx;}


}
