set (PARSER_TEST_EXPECTED_INFO_FILE ${CMAKE_SOURCE_DIR}/ParserTestExpectedResult.txt)
set (LLVM_IR_GENERATOR_TEST_EXPECTED_CODE_FILE ${CMAKE_SOURCE_DIR}/LLVMIRGeneratorTestExpectedResult.txt)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/Defines.hpp.cmake ${CMAKE_CURRENT_SOURCE_DIR}/Defines.hpp)

add_executable (TestLexer TestLexer.cpp)
target_link_libraries (TestLexer CompilingLibrary)
add_test (NAME test_lexer COMMAND TestLexer)

add_executable (TestParser TestParser.cpp)
target_link_libraries (TestParser CompilingLibrary)
add_test (NAME test_parser COMMAND TestParser)

add_executable (TestLLVMIRGenerator TestLLVMIRGenerator.cpp)
target_link_libraries (TestLLVMIRGenerator CompilingLibrary)
add_test (NAME test_llvm_ir_generator COMMAND TestLLVMIRGenerator)
