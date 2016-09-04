#include "Parser.hpp"
#include <assert.h>
#include <iostream>
#include <cctype>
#include <cstdio>
#include <memory>

namespace Compiling
{
namespace Parser
{
void Parser::EatToken ()
{
    lastTokenIndex_++;
    if (lastTokenIndex_ < currentTokensList_.size ())
    {
        currentToken_ = currentTokensList_.at (lastTokenIndex_);
        if (currentToken_.token_ == Lexer::TOKEN_END_OF_INPUT)
            isListed_ = true;
    }
    else
        fprintf (stderr, "Error while parsing: %s\n", "trying to eat token, but all tokens is listed!");
}

int Parser::GetOperatorPrecedence (std::string operatorName)
{
    int precedence = binaryOperatorsPrecedence_ [operatorName];
    if (precedence <= 0)
    {
        binaryOperatorsPrecedence_ [operatorName] = -1;
        return -1;
    }
    else
        return precedence;
}

AST::Expression *Parser::ErrorInExpression (const char *error)
{
    std::cerr << "Error in expression!\n    last_token_index = " << lastTokenIndex_ <<
                 "\n    error: " << error << "\n";
    return 0;
}

AST::Prototype *Parser::ErrorInPrototype (const char *error)
{
    std::cerr << "Error in prototype!\n    last_token_index = " << lastTokenIndex_ <<
                 "\n    error: " << error << "\n";
    return 0;
}

AST::Function *Parser::ErrorInFunction (const char *error)
{
    std::cerr << "Error in function!\n    last_token_index = " << lastTokenIndex_ <<
                 "\n    error: " << error << "\n";
    return 0;
}

AST::Number *Parser::ParseNumber ()
{
    std::cout << "Parsing number... ";
    assert (currentToken_.token_ == Lexer::TOKEN_NUMBER);
    AST::Number *result = new AST::Number (currentToken_.additionalData_);
    std::cout << currentToken_.additionalData_ << " done\n";
    EatToken ();
    return result;
}

AST::Expression *Parser::ParseParentBrackets ()
{
    std::cout << "Parsing parent brackets...";
    assert (currentToken_.token_ == Lexer::TOKEN_OPERATOR);
    assert (currentToken_.additionalData_ == "(");
    EatToken ();
    if (isListed_)
        return ErrorInExpression ("')' expected, but end of input reached!");

    if (currentToken_.token_ == Lexer::TOKEN_OPERATOR && currentToken_.additionalData_ == ")")
        return ErrorInExpression ("empty parent brackets '()'!");

    AST::Expression *expressionInBrackers = ParseExpression ();
    if (!expressionInBrackers)
        return 0;

    if (currentToken_.token_ != Lexer::TOKEN_OPERATOR || currentToken_.additionalData_ != ")")
        return ErrorInExpression ("')' expected!");
    EatToken ();
    std::cout << "done\n";
    return expressionInBrackers;
}

AST::Expression *Parser::ParseIdentifier ()
{
    std::cout << "Parsing identifier...";
    assert (currentToken_.token_ == Lexer::TOKEN_IDENTIFIER);
    std::string identifier = currentToken_.additionalData_;
    std::cout << identifier;
    EatToken ();
    if (currentToken_.token_ == Lexer::TOKEN_OPERATOR && currentToken_.additionalData_ == "(")
    {
        std::cout << " as function call...\n";
        EatToken ();

        if (isListed_)
            return ErrorInExpression ("call arguments and ')' expected, but end of input reached!");

        std::vector <AST::Expression *> arguments;
        std::cout << "Parsing arguments...\n";
        if (currentToken_.token_ != Lexer::TOKEN_OPERATOR || currentToken_.additionalData_ != ")")
        {
            while (true)
            {
                AST::Expression *argument = ParseExpression ();
                if (!argument)
                    return 0;
                arguments.push_back (argument);

                if (isListed_)
                    return ErrorInExpression ("call arguments and ')' expected, but end of input reached!");

                if (currentToken_.token_ == Lexer::TOKEN_OPERATOR && currentToken_.additionalData_ == ")")
                    break;

                if (currentToken_.token_ != Lexer::TOKEN_OPERATOR || currentToken_.additionalData_ != ",")
                    return ErrorInExpression ("',' or ')' expected!" );
                EatToken ();
            }
        }
        std::cout << "done\n";
        EatToken ();
        std::cout << "done\n";
        return new AST::Call (identifier, arguments);
    }
    else
    {
        std::cout << " as variable reference...done\n";
        return new AST::Variable (identifier);
    }
}

AST::Expression *Parser::ParsePrimary ()
{
    std::cout << "Parsing as primary...\n";
    assert (!isListed_);
    if (currentToken_.token_ == Lexer::TOKEN_NUMBER)
        return ParseNumber ();
    else if (currentToken_.token_ == Lexer::TOKEN_OPERATOR && currentToken_.additionalData_ == "(")
        return ParseParentBrackets ();
    else if (currentToken_.token_ == Lexer::TOKEN_IDENTIFIER)
        return ParseIdentifier ();
    else
        return ErrorInExpression ("unknown token when expecting an expression!");
}

AST::Expression *Parser::ParseExpression (int expressionPrecedence)
{
    std::cout << "Parsing expression with start in token " << lastTokenIndex_ << "...\n";
    AST::Expression *left = ParsePrimary ();
    if (!left)
        return 0;
    return ParseTwoArgumentOperatorRight (expressionPrecedence, left);
}

AST::Expression *Parser::ParseTwoArgumentOperatorRight (int expressionPrecedence, AST::Expression *left)
{
    while (true)
    {
        std::cout << "Parsing two argument operator...\n";
        std::cout << "Expression precedence: " << expressionPrecedence << "\n";

        int operatorPrecedence = -1;
        if (currentToken_.token_ == Lexer::TOKEN_OPERATOR)
            operatorPrecedence = GetOperatorPrecedence (currentToken_.additionalData_);

        std::cout << "Operator: " << currentToken_.additionalData_ << " with precedence " << operatorPrecedence << "\n";
        if (operatorPrecedence < expressionPrecedence)
            return left;

        std::string binaryOperatorName = currentToken_.additionalData_;
        EatToken ();
        AST::Expression *right = ParseExpression (operatorPrecedence);
        if (!right)
            return 0;

        int nextOperatorPrecedence = -1;
        if (currentToken_.token_ == Lexer::TOKEN_OPERATOR)
            nextOperatorPrecedence = GetOperatorPrecedence (currentToken_.additionalData_);

        std::cout << "Next operator: " << currentToken_.additionalData_ << " with precedence " << nextOperatorPrecedence << "\n";
        if (operatorPrecedence < nextOperatorPrecedence)
        {
            std::cout << "Parsing next operator args...";
            right = ParseTwoArgumentOperatorRight (operatorPrecedence + 1, right);
            if (!right)
                return 0;
        }
        std::cout << "Done, creating two argument operator AST value..";
        left = new AST::TwoArgumentsOperator (binaryOperatorName, left, right);
    }
}

AST::Prototype *Parser::ParsePrototype ()
{
    std::cout << "Parsing prototype...\n";
    if (currentToken_.token_ != Lexer::TOKEN_IDENTIFIER)
        return ErrorInPrototype ("expected function name in prototype!");

    std::string functionName = currentToken_.additionalData_;
    std::cout << "Prototype function name: " << functionName << "\n";
    EatToken ();

    if (currentToken_.token_ != Lexer::TOKEN_OPERATOR || currentToken_.additionalData_ != "(")
        return ErrorInPrototype ("expected '(' in prototype!");

    EatToken ();
    std::vector <std::string> argumentsNames;
    std::cout << "Reading arguments...\n";
    while (currentToken_.token_ == Lexer::TOKEN_IDENTIFIER)
    {
        argumentsNames.push_back (currentToken_.additionalData_);
        std::cout << "Found argument: " << currentToken_.additionalData_ << "\n";
        EatToken ();

        if (currentToken_.token_ == Lexer::TOKEN_OPERATOR && currentToken_.additionalData_ == ")")
            break;

        if (currentToken_.token_ != Lexer::TOKEN_OPERATOR || currentToken_.additionalData_ != ",")
            return ErrorInPrototype ("',' expected before next argument name!");
        else
            EatToken ();
    }

    std::cout << "Arguments readed!\n";
    if (currentToken_.token_ != Lexer::TOKEN_OPERATOR || currentToken_.additionalData_ != ")")
        return ErrorInPrototype ("')' expected in the end of prototype!");
    EatToken ();
    std::cout << "Prototype successfully readed!\n";
    return new AST::Prototype (functionName, argumentsNames);
}

AST::Function *Parser::ParseFunctionDefinition ()
{
    std::cout << "Parsing function definition...\n";
    assert (currentToken_.token_ == Lexer::TOKEN_DEF_COMMAND);
    EatToken ();

    std::cout << "Parsing prototype...\n";
    AST::Prototype *prototype = ParsePrototype ();
    if (!prototype)
        return 0;

    std::cout << "Parsing body...\n";
    AST::Expression *body = ParseExpression ();
    if (body)
    {
        std::cout << "done\n";
        return new AST::Function (prototype, body);
    }
    else
        return 0;
}

AST::Prototype *Parser::ParseExtern ()
{
    assert (currentToken_.token_ == Lexer::TOKEN_EXTERN_COMMAND);
    std::cout << "Parsing extern...\n";
    EatToken ();
    return ParsePrototype ();
}

AST::Function *Parser::ParseTopLevelExpression ()
{
    std::cout << "Parsing top-level expression...\n";
    AST::Expression *expression = ParseExpression ();
    if (expression)
    {
        AST::Prototype *prototype = new AST::Prototype ("", std::vector <std::string> ());
        return new AST::Function (prototype, expression);
    }
    else
        return 0;
}

Parser::Parser ()
{
    isListed_ = false;
    binaryOperatorsPrecedence_ ["*"] = 2000;
    binaryOperatorsPrecedence_ ["/"] = 2000;
    binaryOperatorsPrecedence_ ["%"] = 2000;
    binaryOperatorsPrecedence_ ["+"] = 1700;
    binaryOperatorsPrecedence_ ["-"] = 1700;
    binaryOperatorsPrecedence_ ["<"] = 1500;
    binaryOperatorsPrecedence_ [">"] = 1400;
    binaryOperatorsPrecedence_ ["<="] = 1300;
    binaryOperatorsPrecedence_ [">="] = 1200;
    binaryOperatorsPrecedence_ ["=="] = 1100;
    binaryOperatorsPrecedence_ ["!="] = 1000;
    binaryOperatorsPrecedence_ ["&&"] = 900;
    binaryOperatorsPrecedence_ [ "||"] = 800;
    binaryOperatorsPrecedence_ ["="] = 700;
    binaryOperatorsPrecedence_ ["*="] = 600;
    binaryOperatorsPrecedence_ ["/="] = 500;
    binaryOperatorsPrecedence_ ["%="] = 400;
    binaryOperatorsPrecedence_ ["+="] = 300;
    binaryOperatorsPrecedence_ ["-="] = 200;
}

AST::TreeNode *Parser::Parse (std::vector <Lexer::TokenData> tokens, bool &resultOutput)
{
    currentTokensList_ = tokens;
    lastTokenIndex_ = -1;
    isListed_ = false;
    EatToken ();

    AST::TreeNode *rootNode = new AST::TreeNode ();
    bool isErrored = false;
    while (!isListed_ && !isErrored)
    {
        AST::TreeValue *nextValue = 0;
        std::cout << "\nParsing new tree value from token " << lastTokenIndex_ << ":\n";

        if (currentToken_.token_ == Lexer::TOKEN_EXTERN_COMMAND)
            nextValue = ParseExtern ();
        else if (currentToken_.token_ == Lexer::TOKEN_DEF_COMMAND)
            nextValue = ParseFunctionDefinition ();
        else
            nextValue = ParseTopLevelExpression ();

        if (!nextValue)
        {
            std::cout << "Parsing stopped by error!\n";
            isErrored = true;
        }
        else
            rootNode->PushValue (nextValue);
    }

    if (isErrored)
        resultOutput = false;
    else
        resultOutput = true;
    return rootNode;
}

Parser::~Parser()
{

}
}
}

