#pragma once
#include <string>
#include <vector>

#define AST_TYPE(typeName) \
    public: \
        virtual std::string GetType () \
        { \
            return #typeName ; \
        }

namespace Compiling
{
namespace Parser
{
namespace AST
{

class TreeValue
{
public:
    virtual std::string GetType () = 0;
    virtual std::string GenerateInfo (std::string indentation) = 0;
    virtual ~TreeValue ();
};

class TreeNode : TreeValue
{
AST_TYPE (TreeNode)
protected:
    std::vector <TreeValue *> childs_;
public:
    TreeNode ();
    void PushValue (TreeValue *value);
    std::vector <TreeValue *> *GetChildsVector ();
    virtual std::string GenerateInfo (std::string indentation);
    virtual ~TreeNode ();
};

/// \brief Interface for all AST expressions.
class Expression : public TreeValue
{
AST_TYPE (Expression)
public:
    Expression ();
    virtual ~Expression ();
};

/// \brief For empty arguments in brackers.
class Empty
{
AST_TYPE (Empty)
public:
    Empty ();
    virtual std::string GenerateInfo (std::string indentation);
    virtual ~Empty ();
};

/// \brief Expression for numbers such as 5.67...
class Number : public Expression
{
AST_TYPE (Number)
protected:
    double number_;
public:
    Number (std::string data);
    double GetNumber ();
    virtual std::string GenerateInfo (std::string indentation);
    virtual ~Number ();
};

/// \brief Expression for variables.
class Variable : public Expression
{
AST_TYPE (Variable)
protected:
    std::string name_;
public:
    Variable (std::string name);
    std::string GetName ();
    virtual std::string GenerateInfo (std::string indentation);
    virtual ~Variable ();
};

/// \brief Expression for binary-argument operators, sych as a + b or a == b.
class TwoArgumentsOperator : public Expression
{
AST_TYPE (TwoArgumentsOperator)
protected:
    std::string operatorName_;
    Expression *left_;
    Expression *right_;
public:
    TwoArgumentsOperator (std::string operatorName, Expression *left, Expression *right);
    std::string GetOperatorName ();
    Expression *GetLeft ();
    Expression *GetRight ();
    virtual std::string GenerateInfo (std::string indentation);
    virtual ~TwoArgumentsOperator ();
};

/// \brief Expression for calling functions.
class Call : public Expression
{
AST_TYPE (Call)
protected:
    std::string functionName_;
    std::vector <Expression *> arguments_;
public:
    Call (std::string functionName, std::vector <Expression *> arguments);
    std::string GetFunctionName ();
    std::vector <Expression *> *GetArgumentsVector ();
    virtual std::string GenerateInfo (std::string indentation);
    virtual ~Call ();
};

/// \brief Represents the function's prototype: name and arguments.
class Prototype : public TreeValue
{
AST_TYPE (Prototype)
protected:
    std::string name_;
    std::vector <std::string> arguments_;
public:
    Prototype (std::string name, std::vector <std::string> arguments);
    std::string GetName ();
    std::vector <std::string> *GetArgumentsVector ();
    virtual std::string GenerateInfo (std::string indentation);
    virtual ~Prototype ();
};

/// \brief Represents a function definition itself.
class Function : public TreeValue
{
AST_TYPE (Function)
protected:
    Prototype *prototype_;
    Expression *body_;
public:
    Function (Prototype *prototype, Expression *body);
    Prototype *GetPrototype ();
    Expression *GetBody ();
    virtual std::string GenerateInfo (std::string indentation);
    virtual ~Function ();
};
}
}
}
