# Based on https://github.com/ttroy50/cmake-examples/tree/master/02-sub-projects/A-basic
# https://cliutils.gitlab.io/modern-cmake/
cmake_minimum_required(VERSION 3.15)

project(REVIDE)

# Enable solution folder support
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Prepend the CMake module search path so we can use our own modules
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)

# Dependencies
set(CMAKE_FOLDER "Dependencies")
include(LLVM)
include(Qt)
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

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    REVIDE/*.cpp
    REVIDE/*.hpp
    REVIDE/*.h
    REVIDE/*.qrc
    REVIDE/*.ui
    REVIDE/*.rc
)

add_executable(${PROJECT_NAME} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${QT_LIBRARIES}
    QScintilla
    LLVM-Wrapper
    ads::qtadvanceddocking
)

if(TARGET ${QT_PACKAGE}::windeployqt)
    # create a target that rebuilds when cmake is re-run
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-windeployqt.c" "void empty() {}")
    add_library(${PROJECT_NAME}-windeployqt STATIC
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-windeployqt.c"
    )
    set_target_properties(${PROJECT_NAME}-windeployqt PROPERTIES
        FOLDER "Qt"
        ARCHIVE_OUTPUT_DIRECTORY "Dependencies"
        PDB_OUTPUT_DIRECTORY "Dependencies"
    )

    # execute windeployqt in a tmp directory after build
    add_custom_command(TARGET ${PROJECT_NAME}-windeployqt
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
        COMMAND ${QT_PACKAGE}::windeployqt --no-compiler-runtime --dir "${CMAKE_CURRENT_BINARY_DIR}/windeployqt" "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )

    # copy deployment directory during installation
    install(
        DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/"
        DESTINATION bin
    )
endif()

# https://www.reddit.com/r/cmake/comments/aewn5s/is_it_a_good_idea_to_use_cmake_include_current/ee534og?utm_source=share&utm_medium=web2x
target_include_directories(${PROJECT_NAME} PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")

target_include_directories(${PROJECT_NAME} PRIVATE "REVIDE" "REVIDE/cutter")

# https://cmake.org/cmake/help/v3.14/manual/cmake-compile-features.7.html#requiring-language-standards
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# Compile a GUI application
#target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS)

# Cutter-specific variables
target_compile_definitions(${PROJECT_NAME} PRIVATE CUTTER_SOURCE_BUILD CUTTER_EXPORT=)

install(TARGETS ${PROJECT_NAME} RUNTIME)

# Set the plugin as the startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

add_subdirectory(REVIDE-Helpers)

# Install VS2019 runtime dependencies
# https://braintrekking.wordpress.com/2013/04/27/dll-hell-how-to-include-microsoft-redistributable-runtime-libraries-in-your-cmakecpack-project/
include(InstallRequiredSystemLibraries)
install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
        DESTINATION bin
        )

set_property(DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY VS_STARTUP_PROJECT REVIDE)

# TODO: https://github.com/LLVMParty/ExtensionRepository/edit/main/extensions.txt
