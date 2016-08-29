#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <CompilingLibrary/Lexer/Lexer.hpp>
#include <CompilingLibrary/Parser/Parser.hpp>
#include <CompilingLibrary/Parser/AST.hpp>
#include "Defines.hpp"

enum ErrorExits
{
    ERROR_WHILE_PARSING = 1,
    ERROR_RESULT_INFO_CODE_DONT_EQUALS_EXPECTED = 2
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
            "# This is small testing code\n"
            "extern inbuilt_func (arg1, arg2)\n"
            "# Multi\n"
            "# line\n"
            "# comment\n"
            "def func1 (arg1, arg2)\n"
            "    x = a + arg2 + (c + arg1) * e / f * h + g\n"
            "\n"
            "func1 (5.87, 2.73)\n";
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
    delete treeRoot;
    if (!result)
        return ERROR_WHILE_PARSING;

    std::ifstream expectedResultFile (PARSER_TEST_EXPECTED_INFO_FILE);
    std::string expectedInfoCode;
    std::string infoCodeLine;
    while (std::getline (expectedResultFile, infoCodeLine))
        expectedInfoCode += infoCodeLine + "\n";
    expectedResultFile.close ();

    if (expectedInfoCode != infoCode)
        return ERROR_RESULT_INFO_CODE_DONT_EQUALS_EXPECTED;
    return 0;
}
