#pragma once

#include "bassuibackend.hpp"

#include <map>
#include <vector>
#include <string>
#include <bass.h> 
#include <bassalac.h> 
#include <bassape.h> 
#include <bassflac.h> 
#include <bassopus.h>
#include <thread>

namespace BASS 
{
    std::string GetVersionStr();
    void BASSError(const char *text, bool isFatal = false, bool showGUI = false);
    std::string GetErrorStr(int err);

    class AudioFormat {
        public:
            enum StreamFormat{
				NullFormat,
                WAV,
                FLAC,
                ALAC,
                AIFF,
                APE,
				AAC,
				M4A,
                MP3,
                MP2,
                MP1,
                OGG,
                OPUS,
				Count // Amount of formats currently defined. 
            };

            static std::map<StreamFormat, std::vector<std::string>> ExtNames;
        
            static StreamFormat GetFormat(std::string fPath);
    };

    class BASSPlayer {
        public:
            enum RepeatMode{
                NoRepeat,
                Repeat,
                RepeatTrack,
                Count,
            };

            // Variables
            static BASSUIBackend * backendQObj;

            // Functions
            static void Init();
            static void Destroy();
            static bool CreateChannelStream(const char fPath[], DWORD *channel);
            static bool StartFilePlayback(const char fPath[], bool queued = false);
            static void StartPausePlayback();
            static void StopPlayback();
            static void StartScroll();
            static void EndScroll();
            static void SetPos(double pos);
            static double GetTrackProgressSecs();
            static std::string GetTrackProgressStr(double pos);
            static void ToggleRepeatMode();

            // Getters & setters.
            static void SetVolume(float vol);
            static float GetVolume();
            static QWORD GetTrackLen();
            static double GetTrackLenSecs();
            static const char *GetTrackLenStr();
            static void SetCurFilePath(const char fPath[]);
            static const char *GetCurFilePath();
            static bool IsPlaying();
            static bool IsScrolling();
            static int GetTrackQueueIdx();
            static int GetNextTrackQueueIdx();
            static int GetPrevTrackQueueIdx();
            static void InitTrackQueue(std::vector<std::string> newQueue);
            static void GoNextTrack();
            static void GoPrevTrack();
            static RepeatMode GetRepeatMode();
            static bool GetShuffleMode();
            static void ToggleShuffleMode();
        
        private:
            // Varibales
            static DWORD curChannel;
            static DWORD queuedChannel;
            static QWORD trackLen;
            static std::string trackLenStr;
            static std::string curFilePath;
            static bool isPlaying;
            static bool isScrolling;
            static std::vector<std::string> trackQueue;
            static int trackQueueIdx;
            
            static int deviceIdx;
            static bool restartChannel;
            static float volume;

            static RepeatMode repeatMode;
            static bool shuffleMode;

            static std::thread queueThread;

            // Functions
            static void QueueNextTrack();
            static void CALLBACK TrackFinishedProcess(HSYNC handle, DWORD channel, DWORD data, void *user);
    };
}
