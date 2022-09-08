# https://github.com/TheLartians/CPM.cmake/releases
include(CPM.cmake)

# Add QScintilla dependency
CPMAddPackage(
	NAME QScintilla
	VERSION 2.13.3
	GIT_REPOSITORY https://github.com/opencor/qscintilla
	DOWNLOAD_ONLY ON
)
if(QScintilla_ADDED)
	# TODO: Qt translations?
	file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
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
		$CACHE{QT_PACKAGE}::Widgets
		$CACHE{QT_PACKAGE}::PrintSupport
	)
	source_group(TREE ${QScintilla_SOURCE_DIR} FILES ${SOURCES})
endif()