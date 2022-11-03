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

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
set(QT_PACKAGE "Qt${QT_VERSION_MAJOR}")
set(QT_LIBRARIES
    ${QT_PACKAGE}::Widgets
    ${QT_PACKAGE}::Svg
)

if("${QT_PACKAGE}" STREQUAL "Qt6")
    set(ADDITIONAL_COMPONENTS Core5Compat OpenGLWidgets)
    list(APPEND QT_LIBRARIES Qt6::Core5Compat Qt6::OpenGLWidgets)
else()
    set(ADDITIONAL_COMPONENTS "")
endif()
find_package(${QT_PACKAGE} COMPONENTS Widgets Svg PrintSupport ${ADDITIONAL_COMPONENTS} REQUIRED)

# https://stackoverflow.com/a/41199492/1806760
# TODO: set VCINSTALLDIR environment variable
# TODO: move to a custom target you can trigger manually
if(${QT_PACKAGE}_FOUND AND WIN32 AND TARGET ${QT_PACKAGE}::qmake AND NOT TARGET ${QT_PACKAGE}::windeployqt)
    get_target_property(_qt_qmake_location ${QT_PACKAGE}::qmake IMPORTED_LOCATION)

    execute_process(
        COMMAND "${_qt_qmake_location}" -query QT_INSTALL_PREFIX
        RESULT_VARIABLE return_code
        OUTPUT_VARIABLE qt_install_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(imported_location "${qt_install_prefix}/bin/windeployqt.exe")

    if(EXISTS ${imported_location})
        add_executable(${QT_PACKAGE}::windeployqt IMPORTED)

        set_target_properties(${QT_PACKAGE}::windeployqt PROPERTIES
            IMPORTED_LOCATION ${imported_location}
        )
    endif()
endif()