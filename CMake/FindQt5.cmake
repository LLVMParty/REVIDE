# Documentation: https://cmake.org/cmake/help/latest/manual/cmake-developer.7.html#find-modules

if(NOT DEFINED REVIDE_DOWNLOAD_QT)
    set(REVIDE_DOWNLOAD_QT ON)
endif()

function(revide_qt5_requested_components_available OUT_VAR)
    set(MISSING_COMPONENTS)
    foreach(COMPONENT IN LISTS Qt5_FIND_COMPONENTS)
        if(NOT TARGET Qt5::${COMPONENT})
            list(APPEND MISSING_COMPONENTS "${COMPONENT}")
        endif()
    endforeach()

    if(MISSING_COMPONENTS)
        set(${OUT_VAR} FALSE PARENT_SCOPE)
        set(Qt5_NOT_FOUND_MESSAGE "Qt5 is missing components: ${MISSING_COMPONENTS}" PARENT_SCOPE)
    else()
        set(${OUT_VAR} TRUE PARENT_SCOPE)
    endif()
endfunction()

set(QT5_CONFIG_FIND_ARGS)
if(Qt5_FIND_VERSION)
    list(APPEND QT5_CONFIG_FIND_ARGS "${Qt5_FIND_VERSION}")
    if(Qt5_FIND_VERSION_EXACT)
        list(APPEND QT5_CONFIG_FIND_ARGS EXACT)
    endif()
endif()
list(APPEND QT5_CONFIG_FIND_ARGS QUIET CONFIG)
if(Qt5_FIND_COMPONENTS)
    list(APPEND QT5_CONFIG_FIND_ARGS COMPONENTS ${Qt5_FIND_COMPONENTS})
endif()

if(Qt5_FOUND)
    revide_qt5_requested_components_available(QT5_COMPONENTS_AVAILABLE)
    if(QT5_COMPONENTS_AVAILABLE)
        return()
    endif()
endif()

find_package(Qt5 ${QT5_CONFIG_FIND_ARGS})
if(Qt5_FOUND)
    revide_qt5_requested_components_available(QT5_COMPONENTS_AVAILABLE)
    if(QT5_COMPONENTS_AVAILABLE)
        if(NOT Qt5_FIND_QUIETLY)
            message(STATUS "Qt5 found: ${Qt5_DIR}")
        endif()
        return()
    endif()
endif()

if(Qt5_FIND_REQUIRED AND REVIDE_DOWNLOAD_QT AND WIN32 AND (MSVC OR CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC"))
    message(STATUS "Downloading Qt5...")

    # Fix warnings about DOWNLOAD_EXTRACT_TIMESTAMP.
    if(POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)
    endif()

    include(FetchContent)

    if(DEFINED FETCHCONTENT_QUIET)
        set(QT5_HAD_FETCHCONTENT_QUIET TRUE)
        set(QT5_OLD_FETCHCONTENT_QUIET "${FETCHCONTENT_QUIET}")
    endif()
    set(FETCHCONTENT_QUIET OFF)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        FetchContent_Declare(Qt5
            URL "https://github.com/x64dbg/deps/releases/download/2025.07.02/qt5.12.12-msvc2017_64.7z"
            URL_HASH SHA256=770490bf09514982c8192ebde9a1fac8821108ba42b021f167bac54e85ada48a
            SOURCE_SUBDIR cmake-do-not-add
        )
    else()
        FetchContent_Declare(Qt5
            URL "https://github.com/x64dbg/deps/releases/download/2025.07.02/qt5.12.12-msvc2017.7z"
            URL_HASH SHA256=3ff2a58e5ed772be475643cd7bb2df3e5499d7169d794ddf1ed5df5c5e862cb6
            SOURCE_SUBDIR cmake-do-not-add
        )
    endif()

    FetchContent_MakeAvailable(Qt5)
    FetchContent_GetProperties(Qt5 SOURCE_DIR QT5_SOURCE_DIR)

    if(QT5_HAD_FETCHCONTENT_QUIET)
        set(FETCHCONTENT_QUIET "${QT5_OLD_FETCHCONTENT_QUIET}")
    else()
        unset(FETCHCONTENT_QUIET)
    endif()

    file(GLOB_RECURSE QT5_CONFIG_CANDIDATES "${QT5_SOURCE_DIR}/*Qt5Config.cmake")
    foreach(QT5_CONFIG_CANDIDATE IN LISTS QT5_CONFIG_CANDIDATES)
        if(QT5_CONFIG_CANDIDATE MATCHES "/lib/cmake/Qt5/Qt5Config\\.cmake$")
            get_filename_component(QT5_CONFIG_DIR "${QT5_CONFIG_CANDIDATE}" DIRECTORY)
            set(Qt5_DIR "${QT5_CONFIG_DIR}" CACHE PATH "Path to Qt5Config.cmake" FORCE)
            break()
        endif()
    endforeach()

    find_package(Qt5 ${QT5_CONFIG_FIND_ARGS})
    if(Qt5_FOUND)
        revide_qt5_requested_components_available(QT5_COMPONENTS_AVAILABLE)
        if(QT5_COMPONENTS_AVAILABLE)
            if(NOT Qt5_FIND_QUIETLY)
                message(STATUS "Qt5 found: ${Qt5_DIR}")
            endif()
            return()
        endif()
    endif()
endif()

set(Qt5_FOUND FALSE)

if(Qt5_FIND_REQUIRED)
    string(JOIN ", " QT5_REQUIRED_COMPONENTS ${Qt5_FIND_COMPONENTS})
    message(FATAL_ERROR
        "Qt5 not found. Required components: ${QT5_REQUIRED_COMPONENTS}. "
        "For MSVC builds, Qt can be auto-downloaded when REVIDE_DOWNLOAD_QT is ON. "
        "For other toolchains, install a compatible Qt SDK and set Qt5_DIR or CMAKE_PREFIX_PATH so CMake can find it."
    )
endif()
