# https://github.com/TheLartians/CPM.cmake/releases
include(CPM.cmake)

# Add VTIL-Core dependency
CPMAddPackage(
	NAME VTIL-Core
	GITHUB_REPOSITORY mrexodia/VTIL-Core
	GIT_TAG nominmax-fix
)

# Add VTIL-NativeLifters dependency
CPMAddPackage(
	NAME VTIL-NativeLifters
	GITHUB_REPOSITORY vtil-project/VTIL-NativeLifters
	GIT_TAG f8837ef4d2412a5a1c25f725f25685a23e1cf1a1
)