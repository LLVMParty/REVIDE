# https://github.com/TheLartians/CPM.cmake/releases
include(CPM.cmake)

# Add VTIL-Core dependency
CPMAddPackage(
	NAME VTIL-Core
	GIT_REPOSITORY https://github.com/mrexodia/VTIL-Core
	GIT_TAG nominmax-fix
)

# Add VTIL-NativeLifters dependency
CPMAddPackage(
	NAME VTIL-NativeLifters
	GIT_REPOSITORY https://github.com/vtil-project/VTIL-NativeLifters
	GIT_TAG f8837ef4d2412a5a1c25f725f25685a23e1cf1a1
)

# Add QScintilla-2.11.6 dependency
CPMAddPackage(
	NAME QScintilla
	VERSION 2.11.6
	GIT_REPOSITORY https://github.com/opencor/qscintilla
	DOWNLOAD_ONLY ON
)
if(QScintilla_ADDED)
	# TODO: Qt translations?
	file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
		${QScintilla_SOURCE_DIR}/Qt4Qt5/*.cpp
		${QScintilla_SOURCE_DIR}/Qt4Qt5/*.h
		${QScintilla_SOURCE_DIR}/src/*.cpp
		${QScintilla_SOURCE_DIR}/src/*.h
		${QScintilla_SOURCE_DIR}/include/*.h
		${QScintilla_SOURCE_DIR}/lexers/*.cpp
		${QScintilla_SOURCE_DIR}/lexers/*.h
		${QScintilla_SOURCE_DIR}/lexlib/*.cpp
		${QScintilla_SOURCE_DIR}/lexlib/*.h
	)
	add_library(QScintilla STATIC ${SOURCES})
	target_include_directories(QScintilla PUBLIC
		${QScintilla_SOURCE_DIR}/include
		${QScintilla_SOURCE_DIR}/lexlib
		${QScintilla_SOURCE_DIR}/Qt4Qt5
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
		Qt5::Widgets
		Qt5::PrintSupport
	)
	source_group(TREE ${QScintilla_SOURCE_DIR} FILES ${SOURCES})
endif()