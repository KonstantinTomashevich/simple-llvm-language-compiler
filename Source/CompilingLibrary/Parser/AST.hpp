#pragma once
#include <string>
#include <vector>
namespace Compiling
{
namespace Parser
{
namespace AST
{

/// \brief Interface for all AST expressions.
class Expression
{
public:
    Expression ();
    virtual std::string GenerateCode () = 0;
    virtual ~Expression ();
};

/// \brief Expression for numbers such as 5.67...
class Number : public Expression
{
protected:
    double number_;
public:
    Number (std::string data);
    virtual std::string GenerateCode ();
    virtual ~Number ();
};

/// \brief Expression for variables.
class Variable : public Expression
{
protected:
    std::string name_;
public:
    Variable (std::string name);
    virtual std::string GenerateCode ();
    virtual ~Variable ();
};

/// \brief Expression for binary-argument operators, sych as a + b or a == b.
class BinaryAngumentsOperator : public Expression
{
protected:
    std::string operatorName_;
    Expression *left_;
    Expression *right_;
public:
    BinaryAngumentsOperator (std::string operatorName, Expression *left, Expression *right);
    virtual std::string GenerateCode ();
    virtual ~BinaryAngumentsOperator ();
};

/// \brief Expression for calling functions.
class Call : public Expression
{
protected:
    std::string functionName_;
    std::vector <Expression *> arguments_;
public:
    Call (std::string functionName, std::vector <Expression *> arguments);
    virtual std::string GenerateCode ();
    virtual ~Call ();
};

/// \brief Represents the function's prototype: name and arguments.
class Prototype
{
protected:
    std::string name_;
    std::vector <std::string> arguments_;
public:
    Prototype (std::string name, std::vector <std::string> arguments);
    virtual std::string GenerateCode ();
    virtual ~Prototype ();
};

/// \brief Represents a function definition itself.
class Function
{
protected:
    Prototype *prototype_;
    Expression *body_;
public:
    Function (Prototype *prototype, Expression *body);
    virtual std::string GenerateCode ();
    virtual ~Function ();
};
}
}
}
