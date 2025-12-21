# VekAmp - dev
<p align="center">
  <img alt="vekamp logo" width="45%" src="assets/branding/wordmark.svg" />
</p>

## About
VekAmp is intended to be a cross platform music player and library management tool making use of the [BASS audio library](https://www.un4seen.com/bass.html). 

The main motivation for this project is to replace my currently used music player (which is Windows exclusive) with one that can be used across several platforms, so that I can get what I'm looking for out of a music player on any platform I may use. I also intend to add a plethora of powerful library management systems to make sure your library is organised and stored in the way you want it to.
>[!NOTE]
>This project is still in it's infancy, If there are any suggestions or feedback you would like to provide, please [submit an issue](https://github.com/vektor451/vekamp/issues/new).

## Current Plans
The plans for the current stage of development are as follows: 
- [x] Basic media playback functionalities. (Play/Pause, Duration/Position, Volume)
- [x] Track Queue.
- [x] More playback functionalities. (Previous, Next, Loop Mode)
- [x] Read metadata.
- [x] Extract album art and display it.
- [ ] Proper port to Linux.

Abandoned plans: 
- Reading Playlists (I'll probably add M3U support later, but the standard is so... non-standardized that it's too much to bother with for now)

The current development branch is [dev](https://github.com/vektor451/vekamp/tree/dev).

## Platforms
This branch has only been tested on windows.

## Building
Building on windows is currently done via QtCreator 18 with Qt 6.10.0, using MSBuild and MSVC from Visual Studio 2026. Other versions of Qt and VS might work, but this is the version that has been currently tested.  
