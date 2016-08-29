#include "AST.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace Compiling
{
namespace Parser
{
namespace AST
{
static const std::string INFO_TAB = "    ";

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

std::string Number::GenerateInfo (std::string indentation)
{
    std::string info;
    info += indentation + "NUMBER ";
    std::ostringstream strs;
    strs << number_;
    info += strs.str () + "\n";
    return info;
}

Number::~Number ()
{
    std::cout << "Deleting number AST value...\n";
}

Variable::Variable (std::string name)
{
    name_ = name;
}

std::string Variable::GenerateCode ()
{
    //TODO: Implement.
}

std::string Variable::GenerateInfo (std::string indentation)
{
    std::string info;
    info += indentation + "VARIABLE " + name_ + "\n";
    return info;
}

Variable::~Variable ()
{
    std::cout << "Deleting variable AST value...\n";
}

TwoArgumentsOperator::TwoArgumentsOperator (std::string operatorName, Expression *left, Expression *right)
{
    operatorName_ = operatorName;
    left_ = left;
    right_ = right;
}

std::string TwoArgumentsOperator::GenerateCode ()
{
    //TODO: Implement.
}

std::string TwoArgumentsOperator::GenerateInfo (std::string indentation)
{
    std::string info;
    info += indentation + "2ARGUMENTS_OPERATOR [ OPERATOR " + operatorName_ + "\n";
    info += indentation + INFO_TAB + "left:\n";
    info += left_->GenerateInfo (indentation + INFO_TAB + INFO_TAB);
    info += indentation + INFO_TAB + "right:\n";
    info += right_->GenerateInfo (indentation + INFO_TAB + INFO_TAB);
    info += indentation + "]\n";
    return info;
}

TwoArgumentsOperator::~TwoArgumentsOperator ()
{
    std::cout << "Deleting two arguments operator AST value...\n";
    delete left_;
    delete right_;
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

std::string Call::GenerateInfo (std::string indentation)
{
    std::string info;
    info += indentation + "CALL\n";
    info += indentation + INFO_TAB + functionName_;

    if (arguments_.empty ())
        info += " [without arguments]\n";
    else
    {
        info += " with arguments [\n";
        for (int index = 0; index < arguments_.size (); index++)
            info += arguments_.at (index)->GenerateInfo (indentation + INFO_TAB + INFO_TAB);
        info += indentation + INFO_TAB + "]\n";
    }
    return info;
}

Call::~Call ()
{
    std::cout << "Deleting function call AST value...\n";
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

std::string Prototype::GenerateInfo (std::string indentation)
{
    std::string info;
    info += indentation + "PROTOTYPE\n";
    info += indentation + INFO_TAB + name_;
    if (arguments_.empty ())
        info += " [without arguments]\n";
    else
    {
        info += " with arguments [\n";
        for (int index = 0; index < arguments_.size (); index++)
            info += indentation + INFO_TAB + INFO_TAB + arguments_.at (index) + "\n";
        info += indentation + INFO_TAB + "]\n";
    }
    return info;
}

Prototype::~Prototype ()
{
    std::cout << "Deleting function prototype AST value...\n";
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

std::string Function::GenerateInfo (std::string indentation)
{
    std::string info;
    info += indentation + "FUNCTION [\n";
    info += prototype_->GenerateInfo (indentation + INFO_TAB);
    info += indentation + INFO_TAB + "BODY\n";
    info += body_->GenerateInfo (indentation + INFO_TAB);
    info += indentation + INFO_TAB + "]\n";
    return info;
}

Function::~Function ()
{
    std::cout << "Deleting function AST value...\n";
    delete prototype_;
    delete body_;
}

Empty::Empty ()
{

}

std::string Empty::GenerateCode ()
{
    return "";
}

std::string Empty::GenerateInfo (std::string indentation)
{
    return indentation + "EMPTY_EXPRESSION\n";
}

Empty::~Empty ()
{
    std::cout << "Deleting empty AST value...\n";
}

TreeNode::TreeNode ()
{

}

void TreeNode::PushValue (TreeValue *value)
{
    childs_.push_back (value);
}

std::string TreeNode::GenerateCode ()
{
    std::string code;
    for (int index = 0; index < childs_.size (); index++)
        code += childs_.at (index)->GenerateCode ();
    return code;
}

std::string TreeNode::GenerateInfo (std::string indentation)
{
    std::string info = "TREE_NODE:\n";
    for (int index = 0; index < childs_.size (); index++)
        info += childs_.at (index)->GenerateInfo (indentation + INFO_TAB) + "\n";
    return info.substr (0, info.size () - 1); // Delete last '\n'
}

TreeNode::~TreeNode ()
{
    std::cout << "Deleting AST node with " << childs_.size () << " childs...\n";
    for (int index = 0; index < childs_.size (); index++)
        delete childs_.at (index);
    childs_.clear ();
}

TreeValue::~TreeValue()
{

}

}
}
}
