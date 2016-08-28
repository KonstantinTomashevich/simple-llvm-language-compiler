file (GLOB_RECURSE Headers *.hpp)
file (GLOB_RECURSE Implementations *.cpp)
add_executable (Compiler ${Headers} ${Implementations})
target_link_libraries (Compiler CompilingLibrary)