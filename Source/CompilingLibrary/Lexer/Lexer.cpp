#include "Lexer.hpp"
#include <assert.h>
#include <iostream>

namespace Compiling
{
namespace Lexer
{
bool Lexer::Step (char &output, int &scanStartIndex, std::string &code)
{
    if (scanStartIndex < code.size ())
    {
        output = code.at (scanStartIndex);
        scanStartIndex += 1;
        std::cout << "";
        return (output != EOF);
    }
    else
        return false;
}

bool Lexer::SkipCommentsAndSpaces (int &scanStartIndex, std::string &code)
{
    bool isComment = false;
    char lastChar;
    do
    {
        if (!Step (lastChar, scanStartIndex, code))
            return false;
        if (lastChar == COMMENT_START_CHAR)
            isComment = true;
        else if (lastChar == COMMENT_END_CHAR)
            isComment = false;
    }
    while (isComment || std::isspace (lastChar) || lastChar == COMMENT_START_CHAR || lastChar == COMMENT_END_CHAR
           || lastChar == '\n' || lastChar == '\r');

    scanStartIndex -= 1;
    return true;
}

TokenData Lexer::ReadAsIdentifier (int &scanStartIndex, std::string &code)
{
    std::string identifier = "";
    char lastChar;
    do
    {
        if (lastChar)
            identifier += lastChar;
        if (!Step (lastChar, scanStartIndex, code))
            return ReturnIdentifierOrCommand (identifier);
    }
    while ((std::isalnum (lastChar) || lastChar == '_') && lastChar != COMMENT_START_CHAR);

    scanStartIndex -= 1;
    return ReturnIdentifierOrCommand (identifier);
}

TokenData Lexer::ReturnIdentifierOrCommand (std::string string)
{
    if (string == "def")
        return TokenData (TOKEN_DEF_COMMAND);
    else if (string == "extern")
        return TokenData (TOKEN_EXTERN_COMMAND);
    else
        return TokenData (TOKEN_IDENTIFIER, string);
}

TokenData Lexer::ReadAsNumber (int &scanStartIndex, std::string &code)
{
    std::string number = "";
    char lastChar;
    do
    {
        if (lastChar)
            number += lastChar;
        if (!Step (lastChar, scanStartIndex, code))
            return TokenData (TOKEN_NUMBER, number);
    }
    while (std::isdigit (lastChar) || lastChar == '.');

    scanStartIndex -= 1;
    return TokenData (TOKEN_NUMBER, number);
}

TokenData Lexer::ReadAsOperator (int &scanStartIndex, std::string &code)
{
    std::string operatorName = "";
    char lastChar;
    do
    {
        if (lastChar)
            operatorName += lastChar;
        if (!Step (lastChar, scanStartIndex, code))
            CorrectIfEmptyBracketsOperator (scanStartIndex, operatorName);
    }
    while (!std::isalnum (lastChar) && !std::isspace (lastChar) && lastChar != COMMENT_START_CHAR &&
           lastChar != '\n' && lastChar != '\r');

    scanStartIndex -= 1;
    return CorrectIfEmptyBracketsOperator (scanStartIndex, operatorName);
}

TokenData Lexer::CorrectIfEmptyBracketsOperator (int &scanStartIndex, std::string operatorName)
{
    if (operatorName == "()" || operatorName == "[]" || operatorName == "{}")
    {
        scanStartIndex -= 1;
        return TokenData (TOKEN_OPERATOR, operatorName.substr (0, 1));
    }
    else
        return TokenData (TOKEN_OPERATOR, operatorName);
}

TokenData Lexer::NextToken (int &scanStartIndex, std::string &code)
{
    if (scanStartIndex >= code.size ())
        return TokenData (TOKEN_END_OF_INPUT);

    char lastChar;
    if (!SkipCommentsAndSpaces (scanStartIndex, code))
        return TokenData (TOKEN_END_OF_INPUT);

    if (!Step (lastChar, scanStartIndex, code))
        return TokenData (TOKEN_END_OF_INPUT);
    scanStartIndex -= 1;

    if (std::isalpha (lastChar))
        return ReadAsIdentifier (scanStartIndex, code);
    else if (std::isdigit (lastChar))
        return ReadAsNumber (scanStartIndex, code);
    else
        return ReadAsOperator (scanStartIndex, code);
}

Lexer::Lexer ()
{

}

std::vector <TokenData> Lexer::LexString (std::string &code)
{
    std::vector <TokenData> tokens;
    int scanStartIndex = 0;
    TokenData lastToken (TOKEN_IDENTIFIER);
    do
    {
        lastToken = NextToken (scanStartIndex, code);
        tokens.push_back (lastToken);
    }
    while (lastToken.token_ != TOKEN_END_OF_INPUT);
    return tokens;
}

Lexer::~Lexer ()
{

}

TokenData::TokenData()
{
    token_ = TOKEN_END_OF_INPUT;
    additionalData_ = "";
}

TokenData::TokenData(Token token, std::string additionData)
{
    token_ = token;
    additionalData_ = additionData;
}

bool TokenData::operator == (TokenData &other)
{
    return (token_ == other.token_ && additionalData_ == other.additionalData_);
}

bool TokenData::operator != (TokenData &other)
{
    return (token_ != other.token_ || additionalData_ != other.additionalData_);
}
}
}
