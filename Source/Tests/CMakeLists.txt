add_executable (TestLexer TestLexer.cpp)
target_link_libraries (TestLexer CompilingLibrary)
add_test (NAME test_lexer COMMAND TestLexer)