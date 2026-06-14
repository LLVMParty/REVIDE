# https://www.kdab.com/wp-content/uploads/stories/QTVTC20-Using-Modern-CMake-Kevin-Funk.pdf
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_GLOBAL_AUTOGEN_TARGET ON)
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Qt")
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "Qt")
set_property(GLOBAL PROPERTY AUTOMOC_SOURCE_GROUP "Qt")
set_property(GLOBAL PROPERTY AUTOMOC_TARGETS_FOLDER "Qt")
set_property(GLOBAL PROPERTY AUTORCC_SOURCE_GROUP "Qt")
set_property(GLOBAL PROPERTY AUTOUIC_SOURCE_GROUP "Qt")

set(REVIDE_QT_VERSION "AUTO" CACHE STRING "Qt major version to use: AUTO, 6, or 5")
set_property(CACHE REVIDE_QT_VERSION PROPERTY STRINGS AUTO 6 5)
string(TOUPPER "${REVIDE_QT_VERSION}" REVIDE_QT_VERSION_NORMALIZED)

set(REVIDE_QT5_COMPONENTS Core Widgets Svg PrintSupport)
set(REVIDE_QT6_COMPONENTS Core Widgets Svg PrintSupport Core5Compat OpenGLWidgets)

if(REVIDE_QT_VERSION_NORMALIZED STREQUAL "6")
    find_package(Qt6 CONFIG REQUIRED COMPONENTS ${REVIDE_QT6_COMPONENTS})
    set(QT_PACKAGE Qt6)
    set(QT_VERSION_MAJOR 6)
elseif(REVIDE_QT_VERSION_NORMALIZED STREQUAL "5")
    find_package(Qt5 REQUIRED COMPONENTS ${REVIDE_QT5_COMPONENTS})
    set(QT_PACKAGE Qt5)
    set(QT_VERSION_MAJOR 5)
elseif(REVIDE_QT_VERSION_NORMALIZED STREQUAL "AUTO")
    find_package(Qt6 CONFIG QUIET COMPONENTS ${REVIDE_QT6_COMPONENTS})
    if(Qt6_FOUND)
        set(QT_PACKAGE Qt6)
        set(QT_VERSION_MAJOR 6)
    else()
        find_package(Qt5 REQUIRED COMPONENTS ${REVIDE_QT5_COMPONENTS})
        set(QT_PACKAGE Qt5)
        set(QT_VERSION_MAJOR 5)
    endif()
else()
    message(FATAL_ERROR "Unsupported REVIDE_QT_VERSION=${REVIDE_QT_VERSION}. Use AUTO, 6, or 5.")
endif()

set(QT_PACKAGE "${QT_PACKAGE}" CACHE STRING "Qt package to use (Qt5/Qt6)" FORCE)
set(QT_LIBRARIES
    ${QT_PACKAGE}::Widgets
    ${QT_PACKAGE}::Svg
)

if(QT_PACKAGE STREQUAL "Qt6")
    list(APPEND QT_LIBRARIES Qt6::Core5Compat Qt6::OpenGLWidgets)
endif()

message(STATUS "Using ${QT_PACKAGE}")

# https://stackoverflow.com/a/41199492/1806760
# TODO: set VCINSTALLDIR environment variable
# TODO: move to a custom target you can trigger manually
if(WIN32 AND TARGET ${QT_PACKAGE}::qmake AND NOT TARGET ${QT_PACKAGE}::windeployqt)
    get_target_property(_qt_qmake_location ${QT_PACKAGE}::qmake IMPORTED_LOCATION)

    execute_process(
        COMMAND "${_qt_qmake_location}" -query QT_INSTALL_PREFIX
        RESULT_VARIABLE return_code
        OUTPUT_VARIABLE qt_install_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(imported_location "${qt_install_prefix}/bin/windeployqt.exe")

    if(EXISTS "${imported_location}")
        add_executable(${QT_PACKAGE}::windeployqt IMPORTED)

        set_target_properties(${QT_PACKAGE}::windeployqt PROPERTIES
            IMPORTED_LOCATION "${imported_location}"
        )
    endif()
endif()
