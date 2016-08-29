#pragma once
#include <CompilingLibrary/Lexer/Lexer.hpp>
#include <string>
#include <vector>
#include <map>
#include "AST.hpp"

namespace Compiling
{
namespace Parser
{
class Parser
{
protected:
    std::map <std::string, int> binaryOperatorsPrecedence_;
    bool isListed_;
    int lastTokenIndex_;
    std::vector <Lexer::TokenData> currentTokensList_;
    Lexer::TokenData currentToken_;
    void EatToken();
    int GetOperatorPrecedence (std::string operatorName);

    AST::Expression *ErrorInExpression (const char *error);
    AST::Prototype *ErrorInPrototype (const char *error);
    AST::Function *ErrorInFunction (const char *error);

    /// \brief such as 5.67
    AST::Number *ParseNumber ();
    /// \brief such as (a + c)
    AST::Expression *ParseParentBrackets ();
    /// \brief such as a_variable or sample_func (arg1, arg2)
    AST::Expression *ParseIdentifier ();
    /// \brief parse number, parent brackets or identifier
    AST::Expression *ParsePrimary ();
    /// \brief top-level function for expression parsing
    AST::Expression *ParseExpression (int expressionPrecedence = 0);
    /// \brief parses binary operators with precedence
    AST::Expression *ParseTwoArgumentOperatorRight (int expressionPrecedence, AST::Expression *left);
    /// \brief parses functions prototypes (defined by def or extern)
    AST::Prototype *ParsePrototype ();
    /// \brief parses function definition (by def)
    AST::Function *ParseFunctionDefinition ();
    /// \brief parses extern function definition
    AST::Prototype *ParseExtern ();
    /// \brief for parsing function bodies with more than one expression
    AST::Function *ParseTopLevelExpression ();
public:
    Parser ();
    AST::TreeNode *Parse (std::vector <Lexer::TokenData> tokens, bool &resultOutput);
    virtual ~Parser ();
};
}
}
