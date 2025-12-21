# VekAmp
<p align="center">
  <img alt="vekamp logo" width="45%" src="assets/branding/wordmark.svg" />
</p>

## About
VekAmp is intended to be a cross platform music player and library management tool making use of the [BASS audio library](https://www.un4seen.com/bass.html). 

The main motivation for this project is to replace my currently used music player (which is Windows exclusive) with one that can be used across several platforms, so that I can get what I'm looking for out of a music player on any platform I may use. I also intend to add a plethora of powerful library management systems to make sure your library is organised and stored in the way you want it to.
>[!NOTE]
>This project is still in it's infancy, If there are any suggestions or feedback you would like to provide, please [submit an issue](https://github.com/vektor451/vekamp/issues/new). 

The current development branch is [dev](https://github.com/vektor451/vekamp/tree/dev).

## Platforms
Currently Supported:
- Windows
- Linux

Planned:
- Android

Potential (I won't put effort into these, as I don't own Apple devices).:
- MacOS
- iOS

## Dependencies
- [BASS with APE, FLAC, OPUS, and ALAC extensions](https://www.un4seen.com/bass.html). The needed headers are currently supplied within the dependencies directory, however you still need to paste in the `.DLL/.LIB` (Windows) or `.SO` (Linux) files into `VekAmp/Dependencies/BASS/Lib`. At least version 2.4.11.0 is required. Older versions may work, but it is not guaranteed.
- [TagLib](https://github.com/taglib/taglib). Likelihood is you have to build this yourself to link it. The default Windows search path for taglib is `C:\taglib`, and the default Linux search path is `~/taglib`. At least version 2.1.1 is required. Older versions may work, but it is not guaranteed.

## Building
Building on Windows is currently done via QtCreator 18 with Qt 6.10.0, requiring MSBuild and MSVC from Visual Studio 2026. These are required in order to link BASS, instead of needing to manually import every function in C. Other versions of Qt and VS might work, but this is the version that has been currently tested. 

Linux also requires QtCreator 18 with Qt 6.10.0, however you can use whatever compiler or generator you like. Only GCC with Ninja has been tested to work, but nothing should require them specifically, unlike on Windows. 
