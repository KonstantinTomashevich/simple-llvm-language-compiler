#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <CompilingLibrary/Lexer/Lexer.hpp>
#include <CompilingLibrary/Parser/Parser.hpp>
#include <CompilingLibrary/Parser/AST.hpp>
#include <CompilingLibrary/Generators/ToLLVM/LLVMGenerator.hpp>
#include "Defines.hpp"

enum ErrorExits
{
    ERROR_WHILE_PARSING = 1,
    ERROR_WHILE_LLVM_IR_CODE_GENERATION = 2,
    ERROR_CODE_DONT_EQUALS_EXPECTED = 3
};

std::string TokenDataToString (Compiling::Lexer::TokenData tokenData)
{
    std::string tokenName;
    switch (tokenData.token_)
    {
    case Compiling::Lexer::TOKEN_END_OF_INPUT : tokenName = "END_OF_INPUT"; break;
    case Compiling::Lexer::TOKEN_DEF_COMMAND : tokenName = "DEF_COMMAND"; break;
    case Compiling::Lexer::TOKEN_EXTERN_COMMAND : tokenName = "EXTERN_COMMAND"; break;

    case Compiling::Lexer::TOKEN_IDENTIFIER : tokenName = "IDENTIFIER"; break;
    case Compiling::Lexer::TOKEN_NUMBER : tokenName = "NUMBER"; break;
    case Compiling::Lexer::TOKEN_OPERATOR : tokenName = "OPERATOR"; break;
    default : tokenName = "UNKNOWN"; break;
    }
    return (tokenName + " : " + tokenData.additionalData_ + "\n");
}


int main (int argumentsCount, char* arguments [])
{
    std::string code =
            "# This is small testing code\n\n"
            "extern sin (angle)\n"
            "extern cos (angle)\n\n"
            "# Multi \n"
            "# Line \n"
            "# Comment \n\n"
            "def function1 (arg1, arg2, arg3)\n"
            "    arg1 + arg2 * arg3 - 6.789 + 1.25 + sin (arg1) * cos (arg2) / sin (arg3)\n\n"
            "def function2 (arg)\n"
            "    function1 (arg * 5.67, cos (arg / 8.9), arg - 2.05)\n\n"
            "function1 (5.6, 3.4, 6.7)\n"
            "function2 (sin (3.4))\n";

    std::cout << "Testing code:\n" << code << "\n\n\n";
    Compiling::Lexer::Lexer lexer;
    std::vector <Compiling::Lexer::TokenData> tokens = lexer.LexString (code);

    std::cout << "Tokens list:\n";
    for (int index = 0; index < tokens.size (); index++)
        std::cout << index << " = " << TokenDataToString (tokens.at (index));
    std::cout << "\n\n\n";

    Compiling::Parser::Parser parser;
    bool result;
    Compiling::Parser::AST::TreeNode *treeRoot = parser.Parse (tokens, result);

    std::string infoCode = treeRoot->GenerateInfo ("");
    std::cout << "\n\n\nResulting AST (by treeRoot->GenerateInfo ()):\n" << infoCode << "\n\n\n";

    if (!result)
    {
        std::cerr << "\n\nFAILED: Error while parsing!\n\n";
        delete treeRoot;
        return ERROR_WHILE_PARSING;
    }

    Compiling::Generators::ToLLVM::LLVMGenerator generator;
    std::string ir = generator.GenerateCode (treeRoot, "test_module", result);
    delete treeRoot;
    std::cout << "\n\n\nGenerated LLVM IR:\n\n" << ir << "\n\n\n";

    if (!result)
    {
        std::cerr << "\n\nFAILED: Error while generating!\n\n";
        return ERROR_WHILE_LLVM_IR_CODE_GENERATION;
    }

    std::ifstream expectedResultFile (LLVM_IR_GENERATOR_TEST_EXPECTED_CODE_FILE);
    std::string expectedCode;
    std::string codeLine;
    while (std::getline (expectedResultFile, codeLine))
        expectedCode += codeLine + "\n";
    expectedResultFile.close ();

    if (ir != expectedCode)
    {
        std::cout << "Generated IR don't equals expected! Expected IR:\n\n" << expectedCode << "\n\n\n";
        std::cout << "FAILED: Generated IR don't equals expected!";
        return ERROR_CODE_DONT_EQUALS_EXPECTED;
    }
    return 0;
}
