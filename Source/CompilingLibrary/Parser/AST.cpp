#include "AST.hpp"
#include <cstdlib>

namespace Compiling
{
namespace Parser
{
namespace AST
{

Expression::Expression ()
{

}

Expression::~Expression ()
{

}

Number::Number (std::string data)
{
    number_ = atof (data.c_str ());
}

std::string Number::GenerateCode ()
{
    //TODO: Implement.
}

Number::~Number ()
{

}

Variable::Variable (std::string name)
{
    name_ = name;
}

std::string Variable::GenerateCode ()
{
    //TODO: Implement.
}

Variable::~Variable ()
{

}

BinaryAngumentsOperator::BinaryAngumentsOperator (std::string operatorName, Expression *left, Expression *right)
{
    operatorName_ = operatorName;
    left_ = left;
    right_ = right;
}

std::string BinaryAngumentsOperator::GenerateCode ()
{
    //TODO: Implement.
}

BinaryAngumentsOperator::~BinaryAngumentsOperator ()
{
    //TODO: Implement.
}

Call::Call (std::string functionName, std::vector <Expression *> arguments)
{
    functionName_ = functionName;
    arguments_ = arguments;
}

std::string Call::GenerateCode ()
{
    //TODO: Implement.
}

Call::~Call ()
{

}

Prototype::Prototype (std::string name, std::vector<std::string> arguments)
{
    name_ = name;
    arguments_ = arguments;
}

std::string Prototype::GenerateCode ()
{
    //TODO: Implement.
}

Prototype::~Prototype ()
{

}

Function::Function (Prototype *prototype, Expression *body)
{
    prototype_ = prototype;
    body_ = body;
}

std::string Function::GenerateCode ()
{
    //TODO: Implement.
}

Function::~Function ()
{
    //TODO: Implement.
}

}
}
}
