#include <string>
#include <vector>
#include <iostream>
#include <CompilingLibrary/Lexer/Lexer.hpp>

enum ErrorExitCodes
{
    ERROR_IN_TOKENS_NUMBER = 1,
    ERROR_IN_TOKEN
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
            "# This is small testing code\n" // 0 tokens
            "extern (inbuilt_func)\n" // 4 tokens, TOKEN_EXTERN_COMMAND, TOKEN_OPERATOR, TOKEN_IDENTIFIER, TOKEN_OPERATOR
            "# Multi\n" // 0 tokens
            "# line\n" // 0 tokens
            "# comment\n" // 0 tokens
            "def func1 ()\n" // 4 tokens, TOKEN_DEF_COMMAND, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_OPERATOR
            "    a = 5.67\n" // 3 tokens, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_NUMBER
            "\n" // 0 tokens
            "func1 ()\n"; // 3 tokens, TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_OPERATOR
            // 1 token, TOKEN_END_OF_FILE
    std::cout << "Testing code:\n" << code << "\n\n\n";
    Compiling::Lexer::Lexer lexer;
    std::vector <Compiling::Lexer::TokenData> tokens = lexer.LexString (code);

    std::cout << "Tokens list:\n";
    for (int index = 0; index < tokens.size (); index++)
        std::cout << index << " = " << TokenDataToString (tokens.at (index));

    if (tokens.size () != 15)
    {
        std::cout << "Error in tokens count: expected 15, but got " << tokens.size () << "!\n";
        return ERROR_IN_TOKENS_NUMBER;
    }

    Compiling::Lexer::TokenData expectedTokens [15];
    expectedTokens [0] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_EXTERN_COMMAND);
    expectedTokens [1] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_OPERATOR, "(");
    expectedTokens [2] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_IDENTIFIER, "inbuilt_func");
    expectedTokens [3] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_OPERATOR, ")");
    expectedTokens [4] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_DEF_COMMAND);
    expectedTokens [5] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_IDENTIFIER, "func1");
    expectedTokens [6] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_OPERATOR, "(");
    expectedTokens [7] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_OPERATOR, ")");
    expectedTokens [8] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_IDENTIFIER, "a");
    expectedTokens [9] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_OPERATOR, "=");
    expectedTokens [10] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_NUMBER, "5.67");
    expectedTokens [11] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_IDENTIFIER, "func1");
    expectedTokens [12] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_OPERATOR, "(");
    expectedTokens [13] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_OPERATOR, ")");
    expectedTokens [14] = Compiling::Lexer::TokenData (Compiling::Lexer::TOKEN_END_OF_INPUT);

    for (int index = 0; index < tokens.size (); index++)
        if (tokens.at (index) != expectedTokens [index])
        {
            std::cout << "Error in tokens:\nToken " << index << " is " <<
                         TokenDataToString (tokens.at (index)) << " but expected " <<
                         TokenDataToString (expectedTokens [index]) << " !\n";
            return ERROR_IN_TOKEN;
        }
    return 0;
}
