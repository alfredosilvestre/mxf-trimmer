
<p align="center">
  <a href="" rel="noopener">
 <img width=219px height=253px src="docs/logo.png" alt="Project logo"></a>
</p>

<h3 align="center">MXF Trimmer</h3>

---

<p align="center"> MXF Trimmer is a trimmer application for MXF files based on PowerVS Core
    <br> 
</p>

## 📝 Table of Contents
- [About](#about)
- [Getting Started](#getting_started)
- [Running the tests](#tests)
- [Usage](#usage)
- [Deployment](#deployment)
- [Built Using](#built_using)
- [TODO](docs/TODO.md)
- [Authors](#authors)
- [Acknowledgments](#acknowledgement)

## 🧐 About <a name = "about"></a>
There are many trimmer applications out there, but almost all of them either don't support MXF files properly or just re-encode the file, making the trimming process very long.

MXF Trimmer trims MXF files preserving the source format and it copies all video/audio frames as is into the final file. It only supports D10 IMX MXF files at the moment.

## 🏁 Getting Started <a name = "getting_started"></a>
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See [deployment](#deployment) for notes on how to deploy the project on a live system.

### Prerequisites

```
Windows machine, tested on Windows 7 and 10
```

### Installing
If you have a licensed copy of Visual Studio 2010, install it and then jump to the [Qt](#Qt) section

#### Compiler
 1. Install [Visual C++ 2010 Express](https://archive.org/download/vs-2010-express-1/VS2010Express1.iso)
 2. Uninstall any Visual C++ 2010 runtime/redistributable newer or equal to 10.0.30319 (Otherwise, it will cause a failure when installing Windows SDK)
 3. Install [Windows SDK 7.1](https://www.microsoft.com/en-us/download/details.aspx?id=8442) (get the GRMSDKX_EN_DVD.iso and make sure to include headers, libraries, tools, compilers and the debugging tools). Make sure .NET framework 4.x is installed, installation of the WinSDK will not allow you to select compilers otherwise.
	- On Windows 10, there is an issue with the .NET framework, so you need to execute the setup from "Setup\SDKSetup.exe" to install the compilers and you may need to install the debugging tools after setup finishes from "Setup\WinSDKDebuggingTools\dbg_x86.exe"
 4. Install [VS 2010 SP1](https://archive.org/download/vs-2010-sp-1dvd-1/VS2010SP1dvd1.iso) for the missing headers (this requires VS Express)
 5. Install [MSVC 2010 SP1 Compiler Update](https://www.microsoft.com/en-au/download/details.aspx?id=4422) for the compiler fixes.

#### Qt <a name = "Qt"></a>
1. Install [Qt 4.8](https://download.qt.io/archive/qt/4.8/4.8.7/qt-opensource-windows-x86-vs2010-4.8.7.exe)
2. Install [Qt Creator](https://download.qt.io/official_releases/qtcreator/4.15/4.15.0/qt-creator-opensource-windows-x86_64-4.15.0.exe)

#### PowerVS Core submodule
If your git client does not download the contents of the PowerVS Core submodule, issue the following command:

```
git submodule update --init --recursive
```

Open the [MXF Trimmer.pro](https://github.com/alfredosilvestre/mxf-trimmer/tree/master/MXF%20Trimmer.pro) file in Qt Creator and build using the standard process.

## 🔧 Running the tests <a name = "tests"></a>
Sadly, no automated tests were created yet so just open a file GUI and test if the functionalities are working.

## 🎈 Usage <a name="usage"></a>
This repository has the functionalities for trimming MXF files, based on [PowerVS Core](https://github.com/alfredosilvestre/powervs-core).

### Interface functionalities
- Trimming D10 IMX30 and IMX50 MXF files
- Reading/writing timecodes
- Change aspect ratio between 4x3 and 16x9
- Audio visualization up to 8 channels (2 at a time)
- Option to visualize and set up trim for another clip while the first trim is processing

### Basic shortcuts
- SPACEBAR -> Play/Pause
- J -> Go to previous frame
- K -> Play/Pause
- L -> Go to next frame
- LEFT -> Go to previous frame
- RIGHT -> Go to next frame
- HOME -> Go to start of file
- END -> Go to end of file
- I -> Mark In
- O -> Mark Out
- SHIFT + I -> Goto In
- SHIFT + O -> Goto Out
- Q -> Goto In
- W -> Goto Out

## :thought_balloon: Development <a name="development"></a>

### Available Defines

* GUI - Enables Graphic User Interface code
* PORTAUDIO - Enables audio output for internal audio card

## 🚀 Deployment <a name = "deployment"></a>
You just need to install [Microsoft Visual C++ 2010 Redistributable Package](https://download.microsoft.com/download/1/6/5/165255E7-1014-4D0A-B094-B6A430A6BFFC/vcredist_x86.exe)
You should also copy the Qt, ffmpeg, portaudio and log4cxx shared libraries to the same location (check the [releases](https://github.com/alfredosilvestre/mxf-trimmer/releases) for examples).

## ⛏️ Built Using <a name = "built_using"></a>
- [Qt 4.8](https://doc.qt.io/archives/qt-4.8/index.html) - Application development framework
- [FFmpeg](https://www.ffmpeg.org/) - Video/Audio encoding/decoding framework
- [PortAudio](https://github.com/PortAudio/portaudio/) - Real-time audio I/O library
- [Log4cxx](https://logging.apache.org/log4cxx/) - Logging framework

## ✍️ Authors <a name = "authors"></a>
- [@alfredosilvestre](https://github.com/alfredosilvestre)


## 🎉 Acknowledgements <a name = "acknowledgement"></a>
- [@kylelobo](https://github.com/kylelobo) - For this doc template :slightly_smiling_face:
- [@aescande](https://github.com/aescande) - For the log4cxx build (forked it [here](https://github.com/alfredosilvestre/log4cxxWin32) also)
- [@brandaopaulo](https://github.com/brandaopaulo) - For the opportunity to work on this project
