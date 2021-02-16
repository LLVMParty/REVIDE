# Based on https://github.com/ttroy50/cmake-examples/tree/master/02-sub-projects/A-basic
# https://cliutils.gitlab.io/modern-cmake/
cmake_minimum_required(VERSION 3.15)

project(REVIDE-Project)

# Enable solution folder support
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Prepend the CMake module search path so we can use our own modules
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)

# LLVM wrapper
set(CMAKE_FOLDER "Dependencies/LLVM")
include(LLVM)
include(Qt)

set(CMAKE_FOLDER "Dependencies")
add_subdirectory(Dependencies)
unset(CMAKE_FOLDER)

# Visual Studio generator specific flags
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    # HACK: DO NOT this to add compiler flags/definitions, use target_compile_options on a
    # target instead https://cmake.org/cmake/help/latest/command/target_compile_options.html

    # Enable multiprocessor compilation
    add_compile_options(/MP)
endif()

# MSVC-specific options
if(MSVC)
    # This assumes the installed LLVM was built in Release mode
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/ZI /Od /Ob0 /DNDEBUG" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/ZI /Od /Ob0 /DNDEBUG" CACHE STRING "" FORCE)

#    if(${LLVM_USE_CRT_RELEASE} STREQUAL "MD")
#        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)
#    elseif(${LLVM_USE_CRT_RELEASE} STREQUAL "MT")
#        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
#    else()
#        message(FATAL_ERROR "Unsupported LLVM_USE_CRT_RELEASE=${LLVM_USE_CRT_RELEASE}")
#    endif()
endif()

add_subdirectory(REVIDE)
add_subdirectory(REVIDE-Helpers)

# Install VS2019 runtime dependencies
# https://braintrekking.wordpress.com/2013/04/27/dll-hell-how-to-include-microsoft-redistributable-runtime-libraries-in-your-cmakecpack-project/
include(InstallRequiredSystemLibraries)
install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
        DESTINATION bin
        )

set_property(DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY VS_STARTUP_PROJECT REVIDE)

# TODO: https://github.com/LLVMParty/ExtensionRepository/edit/main/extensions.txt
