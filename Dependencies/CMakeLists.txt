# https://github.com/TheLartians/CPM.cmake/releases
include(CPM.cmake)

#[[
# Add QScintilla dependency
CPMAddPackage(
	NAME QScintilla
	VERSION 2.13.3
	GIT_REPOSITORY https://github.com/opencor/qscintilla
	DOWNLOAD_ONLY ON
)
if(QScintilla_ADDED)
	# TODO: Qt translations?
	file(GLOB_RECURSE SOURCES
		${QScintilla_SOURCE_DIR}/scintilla/*.cpp
		${QScintilla_SOURCE_DIR}/scintilla/*.h
		${QScintilla_SOURCE_DIR}/src/*.cpp
		${QScintilla_SOURCE_DIR}/src/*.h
	)
	add_library(QScintilla STATIC ${SOURCES})
	target_include_directories(QScintilla PUBLIC
		${QScintilla_SOURCE_DIR}/scintilla/include
		${QScintilla_SOURCE_DIR}/scintilla/lexlib
		${QScintilla_SOURCE_DIR}/scintilla/src
		${QScintilla_SOURCE_DIR}/src
	)
	target_compile_definitions(QScintilla PUBLIC
		SCINTILLA_QT
		SCI_LEXER
		INCLUDE_DEPRECATED_FEATURES
	)
	target_compile_definitions(QScintilla PRIVATE
		QSCINTILLA_MAKE_DLL
	)
	target_link_libraries(QScintilla PUBLIC
		${QT_PACKAGE}::Widgets
		${QT_PACKAGE}::PrintSupport
	)
	source_group(TREE ${QScintilla_SOURCE_DIR} FILES ${SOURCES})
endif()
#]]
CPMAddPackage(
	NAME QtADS
	VERSION 3.8.3
	GIT_REPOSITORY https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System
	GIT_TAG 22f609c
	DOWNLOAD_ONLY ON
	#BUILD_STATIC=OFF
	#BUILD_EXAMPLES=OFF
)
if(QtADS_ADDED)
	file(GLOB SOURCES
		${QtADS_SOURCE_DIR}/src/*.cpp
		${QtADS_SOURCE_DIR}/src/*.h
		${QtADS_SOURCE_DIR}/src/*.qrc
	)
	if(UNIX AND NOT APPLE)
		list(APPEND SOURCES
			${QtADS_SOURCE_DIR}/src/linux/FloatingWidgetTitleBar.cpp
			${QtADS_SOURCE_DIR}/src/linux/FloatingWidgetTitleBar.h
		)
	endif()
	add_library(qtadvanceddocking STATIC ${SOURCES})
	target_compile_options(qtadvanceddocking PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
	if(UNIX AND NOT APPLE)
		target_link_libraries(qtadvanceddocking PUBLIC xcb)
	endif()
	target_compile_features(qtadvanceddocking PUBLIC cxx_std_17)
	target_include_directories(qtadvanceddocking PUBLIC "${QtADS_SOURCE_DIR}/src")
	target_link_libraries(qtadvanceddocking PUBLIC
		${QT_PACKAGE}::Core
		${QT_PACKAGE}::Gui
		${QT_PACKAGE}::Widgets
	)
	target_compile_definitions(qtadvanceddocking PUBLIC ADS_STATIC)
	add_library(ads::qtadvanceddocking ALIAS qtadvanceddocking)
endif()
