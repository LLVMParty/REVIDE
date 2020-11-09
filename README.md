# REVIDE

## Qt

From Git Bash:

```
curl -O -L https://code.qt.io/cgit/qbs/qbs.git/plain/scripts/install-qt.sh 1>nul
sh install-qt.sh --directory /d/Qt --host windows_x86 --target desktop --toolchain win64_msvc2017_64 --version 5.12.9 qt 3d qtactiveqt qtbase qtcanvas3d qtconnectivity qtdeclarative qtgamepad qtgraphicaleffects qtimageformats qtlocation qtm ultimedia qtquickcontrols qtquickcontrols2 qtremoteobjects qtscxml qtsensors qtserialbus qtserialport qtspeech qtsvg qt tools qttranslations qtwebchannel qtwebsockets qtwebview qtwinextras qtxmlpatterns d3dcompiler_47 opengl32sw
```

## LLVM

TODO: Add instructions on how to produce this package.

You can download a prebuilt package for Visual Studio 2019 at https://github.com/LLVMParty/REVIDE/releases/tag/libraries.

## Building

```
vs2019 x64
mkdir build && cd build
cmake -G Ninja -DCMAKE_PREFIX_PATH=d:\Qt\5.12.9\msvc2017_64;d:\CodeBlocks\llvm10-install-full-v2 -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
```