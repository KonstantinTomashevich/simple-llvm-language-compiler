#pragma once
#include <string>
#include <vector>

namespace Compiling
{
namespace Lexer
{
enum Token
{
    TOKEN_END_OF_INPUT = 0,

    TOKEN_DEF_COMMAND,
    TOKEN_EXTERN_COMMAND,

    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR
};

class TokenData
{
public:
    TokenData ();
    TokenData (Token token, std::string additionData = "");
    Token token_;
    std::string additionalData_;

    bool operator == (TokenData &other);
    bool operator != (TokenData &other);
};


const char COMMENT_START_CHAR = '#';
const char COMMENT_END_CHAR = '\n';

class Lexer
{
protected:
    bool SkipCommentsAndSpaces (int &scanStartIndex, std::string &code);
    TokenData ReadAsIdentifier (int &scanStartIndex, std::string &code);
    TokenData ReturnIdentifierOrCommand (std::string string);
    TokenData ReadAsNumber (int &scanStartIndex, std::string &code);
    TokenData ReadAsOperator (int &scanStartIndex, std::string &code);
    TokenData NextToken (int &scanStartIndex, std::string &code);
public:
    Lexer ();
    std::vector <TokenData> LexString (std::string &code);
    virtual ~Lexer ();
};
}
}
