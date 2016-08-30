file (GLOB_RECURSE Headers *.hpp)
file (GLOB_RECURSE Implementations *.cpp)
add_library (CompilingLibrary ${Headers} ${Implementations})
target_link_libraries (CompilingLibrary ${LLVMLibs})
