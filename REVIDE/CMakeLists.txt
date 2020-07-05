project(REVIDE)

# https://www.kdab.com/wp-content/uploads/stories/QTVTC20-Using-Modern-CMake-Kevin-Funk.pdf
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets Svg REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp *.hpp *.h *.qrc *.ui)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Svg LLVM)

# https://www.reddit.com/r/cmake/comments/aewn5s/is_it_a_good_idea_to_use_cmake_include_current/ee534og?utm_source=share&utm_medium=web2x
target_include_directories(${PROJECT_NAME} PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")

# https://cmake.org/cmake/help/v3.14/manual/cmake-compile-features.7.html#requiring-language-standards
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)