#pragma once

#include <map>
#include <vector>
#include <string>
#include <bass.h> 
#include <bassalac.h> 
#include <bassape.h> 
#include <bassflac.h> 
#include <bassopus.h> 

namespace BASS 
{
    std::string GetVersionStr();
    void BASSError(const char *text, bool isFatal = true);

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
            // Variables

            // Functions
            static void Init();
            static void Destroy();
            static void StartFilePlayback(const char fPath[]);
            static void StartPausePlayback();
            static void StopPlayback();
            static void StartScroll();
            static void EndScroll();
            static void SetPos(double pos);
            static double GetTrackProgressSecs();
            static std::string GetTrackProgressStr(double pos);

            // Getters & setters.
            static void SetVolume(float vol);
            static float GetVolume();
            static QWORD GetTrackLen();
            static double GetTrackLenSecs();
            static const char *GetTrackLenStr();
            static void SetCurFilePath(const char fPath[]);
            static const char *GetCurFilePath();
            static bool IsPlaying();
        
        private:
            // Varibales
            static DWORD curChannel;
            static QWORD trackLen;
            static std::string trackLenStr;
            static std::string curFilePath;
            static bool isPlaying;
            
            static int deviceIdx;
            static bool restartChannel;
            static float volume;

            // Functions
    };
}
