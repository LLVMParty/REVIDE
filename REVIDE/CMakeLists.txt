project(REVIDE)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.hpp *.h *.qrc *.ui *.rc)

add_executable(${PROJECT_NAME} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${QT_LIBRARIES}
    QScintilla
    LLVM-Wrapper
)

if(TARGET ${QT_PACKAGE}::windeployqt)
    # create a target that rebuilds when cmake is re-run
    file(TOUCH "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-windeployqt.c")
    add_library(${PROJECT_NAME}-windeployqt STATIC
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-windeployqt.c"
    )
    set_target_properties(${PROJECT_NAME}-windeployqt PROPERTIES
        FOLDER "Qt"
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

target_include_directories(${PROJECT_NAME} PRIVATE "cutter")

# https://cmake.org/cmake/help/v3.14/manual/cmake-compile-features.7.html#requiring-language-standards
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# Compile a GUI application
#target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS)

# Cutter-specific variables
target_compile_definitions(${PROJECT_NAME} PRIVATE CUTTER_SOURCE_BUILD)

install(TARGETS ${PROJECT_NAME} RUNTIME)

# Set the plugin as the startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})