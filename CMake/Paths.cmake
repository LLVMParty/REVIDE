function(revide_set_output_directories output_dir)
    if("${output_dir}" STREQUAL "")
        message(FATAL_ERROR "revide_set_output_directories: output directory must not be empty")
    endif()

    foreach(suffix "" _DEBUG _RELEASE _RELWITHDEBINFO _MINSIZEREL)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${suffix} "${output_dir}" PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY${suffix} "${output_dir}" PARENT_SCOPE)
    endforeach()

    if(MSVC)
        set(VS_DEBUGGER_WORKING_DIRECTORY "${output_dir}" PARENT_SCOPE)
    endif()
endfunction()

option(REVIDE_BUILD_IN_TREE "Output runtime artifacts to source bin/ directory" ON)

if(REVIDE_BUILD_IN_TREE)
    set(REVIDE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
else()
    set(REVIDE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

revide_set_output_directories("${REVIDE_RUNTIME_OUTPUT_DIRECTORY}")
file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
