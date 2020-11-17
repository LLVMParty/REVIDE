# REVIDE

## Qt

From Git Bash:

```
curl -O -L https://code.qt.io/cgit/qbs/qbs.git/plain/scripts/install-qt.sh 1>nul
sh install-qt.sh --directory /d/Qt --host windows_x86 --target desktop --toolchain win64_msvc2017_64 --version 5.12.9 qt 3d qtactiveqt qtbase qtcanvas3d qtconnectivity qtdeclarative qtgamepad qtgraphicaleffects qtimageformats qtlocation qtm ultimedia qtquickcontrols qtquickcontrols2 qtremoteobjects qtscxml qtsensors qtserialbus qtserialport qtspeech qtsvg qt tools qttranslations qtwebchannel qtwebsockets qtwebview qtwinextras qtxmlpatterns d3dcompiler_47 opengl32sw
```

Alternatively you can download and install Qt from [here](https://www.qt.io/offline-installers).

## LLVM

TODO: Add instructions on how to produce this package.

You can download a prebuilt package for Visual Studio 2019 at https://github.com/LLVMParty/REVIDE/releases/tag/libraries.

## Building

You can set the global `CMAKE_PREFIX_PATH` environment variable to contain both Qt and LLVM:

```bash
setx CMAKE_PREFIX_PATH "d:\Qt\5.12.9\msvc2017_64;d:\CodeBlocks\llvm10-install-full-v2"
```

Then run CMake:

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
```

You should also be able to use CLion or Visual Studio to open the repository so you don't have to manually run CMake.