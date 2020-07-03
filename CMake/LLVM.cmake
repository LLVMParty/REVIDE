# This is an INTERFACE target for LLVM, usage:
#   target_link_libraries(${PROJECT_NAME} <PRIVATE|PUBLIC|INTERFACE> LLVM)
# The include directories and compile definitions will be properly handled.

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Find the libraries that correspond to the LLVM components that we wish to use
set(LLVM_LIBRARIES LLVMCore
    LLVMSupport
    LLVMPasses
    LLVMIRReader)

# Split the definitions properly (https://weliveindetail.github.io/blog/post/2017/07/17/notes-setup.html)
separate_arguments(LLVM_DEFINITIONS)

# Some diagnostics (https://stackoverflow.com/a/17666004/1806760)
message(STATUS "LLVM libraries: ${LLVM_LIBRARIES}")
message(STATUS "LLVM includes: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")
message(STATUS "LLVM tools: ${LLVM_TOOLS_BINARY_DIR}")

add_library(LLVM INTERFACE)
target_include_directories(LLVM SYSTEM INTERFACE ${LLVM_INCLUDE_DIRS})
target_link_libraries(LLVM INTERFACE ${LLVM_LIBRARIES})
target_compile_definitions(LLVM INTERFACE ${LLVM_DEFINITIONS} -DNOMINMAX)