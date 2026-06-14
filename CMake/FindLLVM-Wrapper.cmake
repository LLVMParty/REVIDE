# This is an INTERFACE target for LLVM, usage:
#   target_link_libraries(${PROJECT_NAME} <PRIVATE|PUBLIC|INTERFACE> LLVM-Wrapper)
# The include directories and compile definitions will be properly handled.

if(LLVM-Wrapper_FOUND OR TARGET LLVM-Wrapper)
    return()
endif()

if(NOT DEFINED REVIDE_DOWNLOAD_LLVM)
    set(REVIDE_DOWNLOAD_LLVM ON)
endif()

set(CMAKE_FOLDER_LLVM "${CMAKE_FOLDER}")
if(CMAKE_FOLDER)
    set(CMAKE_FOLDER "${CMAKE_FOLDER}/LLVM")
else()
    set(CMAKE_FOLDER "LLVM")
endif()

# Extract the arguments passed to find_package.
# Documentation: https://cmake.org/cmake/help/latest/manual/cmake-developer.7.html#find-modules
set(LLVM_WRAPPER_FIND_ARGS)
if(LLVM-Wrapper_FIND_VERSION)
    list(APPEND LLVM_WRAPPER_FIND_ARGS "${LLVM-Wrapper_FIND_VERSION}")
endif()
if(LLVM-Wrapper_FIND_QUIETLY)
    list(APPEND LLVM_WRAPPER_FIND_ARGS QUIET)
endif()
if(LLVM-Wrapper_FIND_REQUIRED)
    list(APPEND LLVM_WRAPPER_FIND_ARGS REQUIRED)
endif()

# Workaround: prebuilt LLVM 21 on Windows references LibXml2::LibXml2 in
# LLVMWindowsManifest, but LibXml2 may not be installed. Create a dummy
# imported target so CMake does not fail while loading LLVMConfig.cmake.
if(WIN32 AND NOT TARGET LibXml2::LibXml2)
    add_library(LibXml2::LibXml2 INTERFACE IMPORTED)
endif()

if(REVIDE_DOWNLOAD_LLVM)
    set(LLVM_WRAPPER_PROBE_ARGS)
    if(LLVM-Wrapper_FIND_VERSION)
        list(APPEND LLVM_WRAPPER_PROBE_ARGS "${LLVM-Wrapper_FIND_VERSION}")
    endif()
    list(APPEND LLVM_WRAPPER_PROBE_ARGS QUIET)

    find_package(LLVM ${LLVM_WRAPPER_PROBE_ARGS})
    if(NOT LLVM_FOUND)
        include(LLVMPrebuilt)
    endif()
    unset(LLVM_WRAPPER_PROBE_ARGS)
endif()

find_package(LLVM ${LLVM_WRAPPER_FIND_ARGS})
unset(LLVM_WRAPPER_FIND_ARGS)

if(NOT LLVM_FOUND)
    set(LLVM-Wrapper_FOUND FALSE)
    set(CMAKE_FOLDER "${CMAKE_FOLDER_LLVM}")
    unset(CMAKE_FOLDER_LLVM)
    return()
endif()

if(NOT LLVM-Wrapper_FIND_QUIETLY)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
endif()

# Split the definitions properly (https://weliveindetail.github.io/blog/post/2017/07/17/notes-setup.html)
separate_arguments(LLVM_DEFINITIONS)

# Support dynamically-linked LLVM.dll in LLVMParty's Windows prebuilts. Some
# LLVM installs expose LLVM.dll without defining an imported LLVM target, but
# CMake needs an imported SHARED target for runtime DLL deployment.
if(WIN32 AND EXISTS "${LLVM_TOOLS_BINARY_DIR}/LLVM.dll" AND NOT TARGET LLVM)
    find_library(LLVM_WRAPPER_LLVM_IMPLIB LLVM PATHS ${LLVM_LIBRARY_DIRS} NO_DEFAULT_PATH REQUIRED)
    add_library(LLVM SHARED IMPORTED)
    set_target_properties(LLVM PROPERTIES
        IMPORTED_IMPLIB "${LLVM_WRAPPER_LLVM_IMPLIB}"
        IMPORTED_LOCATION "${LLVM_TOOLS_BINARY_DIR}/LLVM.dll"
    )
    unset(LLVM_WRAPPER_LLVM_IMPLIB)
endif()

# https://github.com/JonathanSalwan/Triton/issues/1082#issuecomment-1030826696
if(LLVM_LINK_LLVM_DYLIB)
    set(LLVM-Wrapper_LIBS LLVM)
elseif(LLVM-Wrapper_FIND_COMPONENTS)
    if(NOT LLVM-Wrapper_FIND_QUIETLY)
        message(STATUS "LLVM components: ${LLVM-Wrapper_FIND_COMPONENTS}")
    endif()
    llvm_map_components_to_libnames(LLVM-Wrapper_LIBS ${LLVM-Wrapper_FIND_COMPONENTS})
else()
    set(LLVM-Wrapper_LIBS ${LLVM_AVAILABLE_LIBS})
endif()

# Some diagnostics (https://stackoverflow.com/a/17666004/1806760)
if(NOT LLVM-Wrapper_FIND_QUIETLY)
    message(STATUS "LLVM libraries: ${LLVM-Wrapper_LIBS}")
    message(STATUS "LLVM includes: ${LLVM_INCLUDE_DIRS}")
    message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")
    message(STATUS "LLVM tools: ${LLVM_TOOLS_BINARY_DIR}")
endif()

add_library(LLVM-Wrapper INTERFACE IMPORTED)
target_include_directories(LLVM-Wrapper SYSTEM INTERFACE ${LLVM_INCLUDE_DIRS})
target_link_libraries(LLVM-Wrapper INTERFACE ${LLVM-Wrapper_LIBS})
target_compile_definitions(LLVM-Wrapper INTERFACE ${LLVM_DEFINITIONS})

# Set the appropriate minimum C++ standard.
if(LLVM_VERSION VERSION_GREATER_EQUAL "16.0.0")
    # https://releases.llvm.org/16.0.0/docs/CodingStandards.html#c-standard-versions
    target_compile_features(LLVM-Wrapper INTERFACE cxx_std_17)
elseif(LLVM_VERSION VERSION_GREATER_EQUAL "10.0.0")
    # https://releases.llvm.org/10.0.0/docs/CodingStandards.html#c-standard-versions
    target_compile_features(LLVM-Wrapper INTERFACE cxx_std_14)
else()
    # https://releases.llvm.org/9.0.0/docs/CodingStandards.html#c-standard-versions
    target_compile_features(LLVM-Wrapper INTERFACE cxx_std_11)
endif()

if(WIN32)
    target_compile_definitions(LLVM-Wrapper INTERFACE NOMINMAX)

    # This target has an unnecessary diaguids.lib embedded in the installation.
    if(TARGET LLVMDebugInfoPDB)
        get_target_property(LLVMDebugInfoPDB_LIBS LLVMDebugInfoPDB INTERFACE_LINK_LIBRARIES)
        foreach(LLVMDebugInfoPDB_LIB ${LLVMDebugInfoPDB_LIBS})
            if(LLVMDebugInfoPDB_LIB MATCHES "diaguids.lib")
                list(REMOVE_ITEM LLVMDebugInfoPDB_LIBS "${LLVMDebugInfoPDB_LIB}")
                break()
            endif()
        endforeach()
        set_target_properties(LLVMDebugInfoPDB PROPERTIES
            INTERFACE_LINK_LIBRARIES "${LLVMDebugInfoPDB_LIBS}"
        )
        unset(LLVMDebugInfoPDB_LIBS)
    endif()
endif()

set(CMAKE_FOLDER "${CMAKE_FOLDER_LLVM}")
unset(CMAKE_FOLDER_LLVM)

set(LLVM-Wrapper_FOUND ON)
