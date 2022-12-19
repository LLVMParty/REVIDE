# REVIDE

![screenshot of the UI](https://i.imgur.com/xs3lxA9.png)

## Dependencies

You can get precompiled dependencies for MSVC here: https://github.com/LLVMParty/REVIDE/releases/tag/libraries

### Qt

From Git Bash:

```
curl -O -L https://code.qt.io/cgit/qbs/qbs.git/plain/scripts/install-qt.sh 1>nul
sh install-qt.sh --directory /d/Qt --host windows_x86 --target desktop --toolchain win64_msvc2017_64 --version 5.12.9 qt 3d qtactiveqt qtbase qtcanvas3d qtconnectivity qtdeclarative qtgamepad qtgraphicaleffects qtimageformats qtlocation qtm ultimedia qtquickcontrols qtquickcontrols2 qtremoteobjects qtscxml qtsensors qtserialbus qtserialport qtspeech qtsvg qt tools qttranslations qtwebchannel qtwebsockets qtwebview qtwinextras qtxmlpatterns d3dcompiler_47 opengl32sw
```

Alternatively you can download and install Qt from [here](https://www.qt.io/offline-installers).

On macos (M1) you can install it with `brew install qt@6`. You can find the prefix with `brew --prefix qt@6`.

### LLVM

TODO: Add instructions on how to produce this package.

On macos you can install LLVM with `brew install llvm@15`. You can find the prefix with `brew --prefix llvm@15`.

For Windows you can download precompiled dependencies (LLVM 15 and Qt 5.12.12) [here](https://github.com/LLVMParty/REVIDE/releases/tag/libraries).

## Building (generic)

You have to set the [CMAKE_PREFIX_PATH](https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html) CMake variable on the CMake command line to a `;`-separated list of prefixes for the dependencies:

```bash
cmake -B build "-DCMAKE_PREFIX_PATH=/path/to/llvm;/path/to/qt"
cmake --build build --parallel --config RelWithDebInfo
```

It is important to surround the argument with quotes on Unix platforms, because the `;` appears to have a special meaning.

## Building (macos)

```sh
brew install llvm@15 qt@6
cmake -B build "-DCMAKE_PREFIX_PATH=$(brew --prefix llvm@15);$(brew --prefix qt@6)"
cmake --build build --parallel
```
