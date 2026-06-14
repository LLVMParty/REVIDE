include(FetchContent)

set(REVIDE_QTADS_GIT_REPOSITORY "https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System" CACHE STRING "Qt Advanced Docking System Git repository")
set(REVIDE_QTADS_GIT_TAG "4.5.0" CACHE STRING "Qt Advanced Docking System Git tag or commit")

if(NOT REVIDE_DOWNLOAD_QTADS)
    message(FATAL_ERROR "QtADS is fetched with FetchContent. Enable REVIDE_DOWNLOAD_QTADS to configure this project.")
endif()

if(DEFINED FETCHCONTENT_QUIET)
    set(QTADS_HAD_FETCHCONTENT_QUIET TRUE)
    set(QTADS_OLD_FETCHCONTENT_QUIET "${FETCHCONTENT_QUIET}")
endif()
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(QtADS
    GIT_REPOSITORY "${REVIDE_QTADS_GIT_REPOSITORY}"
    GIT_TAG "${REVIDE_QTADS_GIT_TAG}"
    SOURCE_SUBDIR cmake-do-not-add
)
FetchContent_MakeAvailable(QtADS)
FetchContent_GetProperties(QtADS SOURCE_DIR QtADS_SOURCE_DIR)

if(QTADS_HAD_FETCHCONTENT_QUIET)
    set(FETCHCONTENT_QUIET "${QTADS_OLD_FETCHCONTENT_QUIET}")
else()
    unset(FETCHCONTENT_QUIET)
endif()

file(GLOB QTADS_SOURCES CONFIGURE_DEPENDS
    ${QtADS_SOURCE_DIR}/src/*.cpp
    ${QtADS_SOURCE_DIR}/src/*.h
    ${QtADS_SOURCE_DIR}/src/*.qrc
)
if(UNIX AND NOT APPLE)
    list(APPEND QTADS_SOURCES
        ${QtADS_SOURCE_DIR}/src/linux/FloatingWidgetTitleBar.cpp
        ${QtADS_SOURCE_DIR}/src/linux/FloatingWidgetTitleBar.h
    )
endif()

add_library(qtadvanceddocking STATIC ${QTADS_SOURCES})
if(MSVC)
    target_compile_options(qtadvanceddocking PUBLIC /utf-8)
endif()
if(UNIX AND NOT APPLE)
    target_link_libraries(qtadvanceddocking PUBLIC xcb)
endif()
target_compile_features(qtadvanceddocking PUBLIC cxx_std_17)
target_include_directories(qtadvanceddocking PUBLIC "${QtADS_SOURCE_DIR}/src")
target_link_libraries(qtadvanceddocking PUBLIC
    ${QT_PACKAGE}::Core
    ${QT_PACKAGE}::Gui
    ${QT_PACKAGE}::Widgets
    ${QT_PACKAGE}::PrintSupport
)
target_compile_definitions(qtadvanceddocking PUBLIC ADS_STATIC)
source_group(TREE ${QtADS_SOURCE_DIR} FILES ${QTADS_SOURCES})
add_library(ads::qtadvanceddocking ALIAS qtadvanceddocking)
