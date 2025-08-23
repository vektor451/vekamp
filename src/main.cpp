// c++
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// vekamp
#include "utils.hpp"

// libs and lib helpers
#include "bassplayer.hpp"

// globals, move this to it's own class eventually. 
std::string FileName;

int main(int argc, char *argv[])
{
    BASS::BASSPlayer::Init();

    printf("Path: %s\n", argv[1]);

    if(argc < 2)
    {
        printf("No path specified.\n");
		FileName = "No file.";
        //return 0;
    } else {
		FileName = argv[1];
	}

	BASS::BASSPlayer::StartFilePlayback(FileName.c_str());
	BASS::AudioFormat::StreamFormat format = BASS::AudioFormat::GetFormat(FileName);
	printf("Format: %d", (int)format);
}