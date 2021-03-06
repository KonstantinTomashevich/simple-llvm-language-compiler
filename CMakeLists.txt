cmake_minimum_required (VERSION 2.8.6)
if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        # INTERFACE_LINK_LIBRARIES defines the link interface
        cmake_policy (SET CMP0022 NEW)
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        # Disallow use of the LOCATION target property - therefore we set to OLD as we still need it
        cmake_policy (SET CMP0026 OLD)
        # MACOSX_RPATH is enabled by default
        cmake_policy (SET CMP0042 NEW)
    endif ()
endif ()

project (SimpleLLVMLanguageCompiler)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
enable_testing ()


if (NOT LLVM_INSTALL_DIR)
    message (WARNING "Specify LLVM_INSTALL_DIR!")
endif ()

if (LLVM_INSTALL_DIR)
    set (LLVM_DIR ${LLVM_INSTALL_DIR}/share/llvm/cmake/)
endif ()
find_package (LLVM REQUIRED CONFIG)

include_directories (${LLVM_INCLUDE_DIRS})
add_definitions (${LLVM_DEFINITIONS})
llvm_map_components_to_libnames (LLVMLibs support core irreader)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory (Source)
