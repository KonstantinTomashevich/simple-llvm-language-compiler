#include "Lexer.hpp"
#include <assert.h>
#include <iostream>

namespace Compiling
{
namespace Lexer
{
bool Lexer::SkipCommentsAndSpaces (int &scanStartIndex, std::string &code)
{
    bool isComment = false;
    char lastChar = -1;
    do
    {
        if (lastChar != -1)
            scanStartIndex++;

        if (scanStartIndex < code.size ())
            lastChar = code.at (scanStartIndex);
        else
            return false;

        if (lastChar == COMMENT_START_CHAR)
            isComment = true;

        else if (lastChar == COMMENT_END_CHAR)
            isComment = false;
    }
    while (isComment || std::isspace (lastChar) || lastChar == COMMENT_START_CHAR || lastChar == COMMENT_END_CHAR
           || lastChar == '\n' || lastChar == '\r');

    return true;
}

TokenData Lexer::ReadAsIdentifier (int &scanStartIndex, std::string &code)
{
    std::string identifier = "";
    char lastChar = -1;
    do
    {
        if (lastChar != -1)
        {
            identifier += lastChar;
            scanStartIndex++;
        }

        if (scanStartIndex < code.size ())
            lastChar = code.at (scanStartIndex);
        else
            return ReturnIdentifierOrCommand (identifier);
    }
    while ((std::isalnum (lastChar) || lastChar == '_') && lastChar != COMMENT_START_CHAR);

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
    char lastChar = -1;
    do
    {
        if (lastChar != -1)
        {
            number += lastChar;
            scanStartIndex++;
        }
        if (scanStartIndex < code.size ())
            lastChar = code.at (scanStartIndex);
        else
            return TokenData (TOKEN_NUMBER, number);
    }
    while (std::isdigit (lastChar) || lastChar == '.');

    return TokenData (TOKEN_NUMBER, number);
}

TokenData Lexer::ReadAsOperator (int &scanStartIndex, std::string &code)
{
    std::string operatorName = "";
    char lastChar = -1;
    do
    {
        if (lastChar != -1)
        {
            operatorName += lastChar;
            scanStartIndex++;
        }

        // For correct reading of empty brackets, for example "SampleFunction ()".
        if (operatorName == "(" || operatorName == "[" || operatorName == "{" ||
                operatorName == ")" || operatorName == "]" || operatorName == "}")
            return TokenData (TOKEN_OPERATOR, operatorName);

        if (scanStartIndex < code.size ())
            lastChar = code.at (scanStartIndex);
        else
            return TokenData (TOKEN_OPERATOR, operatorName);
    }
    while (!std::isalnum (lastChar) && !std::isspace (lastChar) && lastChar != COMMENT_START_CHAR &&
           lastChar != '\n' && lastChar != '\r');

    return TokenData (TOKEN_OPERATOR, operatorName);
}

TokenData Lexer::NextToken (int &scanStartIndex, std::string &code)
{
    if (scanStartIndex >= code.size ())
        return TokenData (TOKEN_END_OF_INPUT);

    char lastChar;
    if (!SkipCommentsAndSpaces (scanStartIndex, code))
        return TokenData (TOKEN_END_OF_INPUT);

    if (scanStartIndex < code.size ())
        lastChar = code.at (scanStartIndex);
    else
        return TokenData (TOKEN_END_OF_INPUT);

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
