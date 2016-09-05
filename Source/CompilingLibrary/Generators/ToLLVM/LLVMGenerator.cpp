#include "LLVMGenerator.hpp"
#include <iostream>
#include <assert.h>
#include <sstream>
#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <llvm/Support/raw_ostream.h>

namespace Compiling
{
namespace Generators
{
namespace ToLLVM
{
llvm::Value *LLVMGenerator::ErrorInCodeGeneration (const char *error)
{
    std::cerr << "Error in LLVM IR code generation: " << error << "\n";
    return 0;
}

llvm::Function *LLVMGenerator::ErrorInFunctionCodeGeneration (const char *error)
{
    std::cerr << "Error in LLVM IR code generation: " << error << "\n";
    return 0;
}

llvm::Value *LLVMGenerator::GenerateValue (Parser::AST::Expression *value)
{
    assert (value);
    if (value->GetType () == "Number")
        return GenerateNumber ((Parser::AST::Number *) value);
    else if (value->GetType () == "Variable")
        return GenerateVariable ((Parser::AST::Variable *) value);
    else if (value->GetType () == "TwoArgumentsOperator")
        return GenerateTwoArgumentsOperator ((Parser::AST::TwoArgumentsOperator *) value);
    else if (value->GetType () == "Call")
        return GenerateCall ((Parser::AST::Call *) value);
    else
        ErrorInCodeGeneration ("unknown value type!");
}

llvm::Value *LLVMGenerator::GenerateNumber (Parser::AST::Number *number)
{
    assert (number);
    assert (number->GetType () == "Number");
    return llvm::ConstantFP::get (llvm::getGlobalContext (), llvm::APFloat (number->GetNumber ()));
}

llvm::Value *LLVMGenerator::GenerateVariable (Parser::AST::Variable *variable)
{
    assert (variable);
    assert (variable->GetType () == "Variable");
    llvm::Value *variableValue = namedValues_ [variable->GetName ()];
    if (variableValue)
        return variableValue;
    else
        return ErrorInCodeGeneration ("unknown variable name!");
}

llvm::Value *LLVMGenerator::GenerateTwoArgumentsOperator (Parser::AST::TwoArgumentsOperator *operatorAST)
{
    assert (operatorAST);
    assert (operatorAST->GetType () == "TwoArgumentsOperator");

    std::string operatorName = operatorAST->GetOperatorName ();
    if (operatorName == "=")
    {
        //TODO: Implement
        /*if (operatorAST->GetLeft ()->GetType () != "Variable")
            return ErrorInCodeGeneration ("Left expression of \"=\" binary operator will be variable!");

        llvm::Value *valueToSet = namedValues_ [variable->GetName ()];
        if (!valueToSet)
        {

        }*/
        return ErrorInCodeGeneration ("\"=\" isn't supported now!");
    }

    llvm::Value *left = GenerateValue (operatorAST->GetLeft ());
    llvm::Value *right = GenerateValue (operatorAST->GetRight ());

    if (!left || !right)
        return 0;

    if (operatorName == "+")
        return builder_.CreateFAdd (left, right, "temporary_add_function");
    else if (operatorName == "-")
        return builder_.CreateFSub (left, right, "temporary_sub_function");
    else if (operatorName == "*")
        return builder_.CreateFMul (left, right, "temporary_multiply_function");
    else if (operatorName == "/")
        return builder_.CreateFDiv (left, right, "temporary_div_function");

    else if (operatorName == "<")
    {
        left = builder_.CreateFCmpULT (left, right, "temporary_compare_smaller_function");
        return builder_.CreateUIToFP (left, llvm::Type::getDoubleTy (llvm::getGlobalContext ()), "temporary_boolean");
    }

    else if (operatorName == ">")
    {
        left = builder_.CreateFCmpUGT (left, right, "temporary_compare_bigger_function");
        return builder_.CreateUIToFP (left, llvm::Type::getDoubleTy (llvm::getGlobalContext ()), "temporary_boolean");
    }

    else if (operatorName == "==")
    {
        left = builder_.CreateFCmpUEQ (left, right, "temporary_compare_equal_function");
        return builder_.CreateUIToFP (left, llvm::Type::getDoubleTy (llvm::getGlobalContext ()), "temporary_boolean");
    }

    else if (operatorName == "<=")
    {
        left = builder_.CreateFCmpULE (left, right, "temporary_compare_smaller_or_equal_function");
        return builder_.CreateUIToFP (left, llvm::Type::getDoubleTy (llvm::getGlobalContext ()), "temporary_boolean");
    }

    else if (operatorName == ">=")
    {
        left = builder_.CreateFCmpUGE (left, right, "temporary_compare_bigger_or_equal_function");
        return builder_.CreateUIToFP (left, llvm::Type::getDoubleTy (llvm::getGlobalContext ()), "temporary_boolean");
    }

    else if (operatorName == "!=")
    {
        left = builder_.CreateFCmpUNE (left, right, "temporary_compare_not_equal_function");
        return builder_.CreateUIToFP (left, llvm::Type::getDoubleTy (llvm::getGlobalContext ()), "temporary_boolean");
    }
    else
        return ErrorInCodeGeneration ((
                                          std::string ("unknown two arguments operator: \"") + operatorName +
                                          std::string ("\"!\n")).c_str ());
}

llvm::Value *LLVMGenerator::GenerateCall (Parser::AST::Call *call)
{
    assert (call);
    assert (call->GetType () == "Call");
    llvm::Function *functionToCall = module_->getFunction (call->GetFunctionName ());
    if (!functionToCall)
        return ErrorInCodeGeneration ((
                                          std::string ("trying to call unknown function: ") + call->GetFunctionName () +
                                          std::string ("!\n")).c_str ());

    if (functionToCall->arg_size () != call->GetArgumentsVector ()->size ())
    {
        std::ostringstream argsCount;
        argsCount << functionToCall->arg_size ();

        std::ostringstream argsGot;
        argsGot << call->GetArgumentsVector ()->size ();

        return ErrorInCodeGeneration ((
                                          std::string ("incorrect number of arguments "
                                                       "in call of: ") + call->GetFunctionName () +
                                          std::string ("! Expected:") + argsCount.str () +
                                          std::string (", but got: ") + argsGot.str () +
                                          std::string ("!\n")).c_str ());
    }

    std::vector <llvm::Value *> argumentsValues;
    for (int index = 0; index < call->GetArgumentsVector ()->size (); index++)
    {
        llvm::Value *argumentValue = GenerateValue (call->GetArgumentsVector ()->at (index));
        if (!argumentValue)
            return 0;
        argumentsValues.push_back (argumentValue);
    }
    return builder_.CreateCall (functionToCall, argumentsValues, (call->GetFunctionName () + "_temporary_call").c_str ());
}

llvm::Function *LLVMGenerator::GeneratePrototype (Parser::AST::Prototype *prototype)
{
    assert (prototype);
    assert (prototype->GetType () == "Prototype");
    std::vector <llvm::Type *> types (prototype->GetArgumentsVector ()->size (),
                                      llvm::Type::getDoubleTy (llvm::getGlobalContext ()));

    llvm::FunctionType *functionType = llvm::FunctionType::get (
                llvm::Type::getDoubleTy (llvm::getGlobalContext ()), types, false);

    llvm::Function *function = llvm::Function::Create (functionType, llvm::Function::ExternalLinkage,
                                                       prototype->GetName (), module_);

    if (function->getName () != prototype->GetName ())
    {
        function->eraseFromParent ();
        function = module_->getFunction (prototype->GetName ());

        if (!function->empty ())
            return ErrorInFunctionCodeGeneration ((
                                                      std::string ("redefinition of function: ") + prototype->GetName () +
                                                      std::string ("!\n")).c_str ());

        if (function->arg_size () != prototype->GetArgumentsVector ()->size ())
            return ErrorInFunctionCodeGeneration ((
                                                      std::string ("redefinition of function with "
                                                                   "different count of arguments: ") + prototype->GetName () +
                                                      std::string ("!\n")).c_str ());;
    }

    int index = 0;
    for (llvm::Function::arg_iterator argumentsIterator = function->arg_begin ();
         index < prototype->GetArgumentsVector ()->size ();
         argumentsIterator++, index++)
        argumentsIterator->setName (prototype->GetArgumentsVector ()->at (index));


    return function;
}

llvm::Function *LLVMGenerator::GenerateFunction (Parser::AST::Function *functionAST)
{
    assert (functionAST);
    assert (functionAST->GetType () == "Function");
    llvm::Function *function  = module_->getFunction (functionAST->GetPrototype ()->GetName ());
    if (!function)
        function = GeneratePrototype (functionAST->GetPrototype ());
    if (!function)
        return 0;

    llvm::BasicBlock *block = llvm::BasicBlock::Create (llvm::getGlobalContext (), "entry", function);
    builder_.SetInsertPoint (block);

    namedValues_.clear ();
    for (auto &argument : function->args ())
        namedValues_ [argument.getName ()] = &argument;

    llvm::Value *body = GenerateValue (functionAST->GetBody ());
    if (body)
    {
        builder_.CreateRet (body);
        llvm::verifyFunction (*function);
        return function;
    }
    else
    {
        function->eraseFromParent ();
        return 0;
    }
}

LLVMGenerator::LLVMGenerator () : builder_ (llvm::getGlobalContext ()), module_ (0)
{

}

std::string LLVMGenerator::GenerateCode (Parser::AST::TreeNode *rootASTNode, char *moduleName, bool &result)
{
    module_ = new llvm::Module (moduleName, llvm::getGlobalContext ());
    bool errored = false;
    for (int index = 0; index < rootASTNode->GetChildsVector ()->size () && !errored; index++)
    {
        Parser::AST::TreeValue *value = rootASTNode->GetChildsVector ()->at (index);
        if (value->GetType () == "Prototype")
        {
            if (!GeneratePrototype ((Parser::AST::Prototype *) value))
                errored = true;
        }
        else if (value->GetType () == "Function")
        {
            if (!GenerateFunction ((Parser::AST::Function *) value))
                errored = true;
        }
        else
        {
            std::cerr << "\nUnknown tree value type: " << value->GetType () << "! Tree child index: " << index << ".\n";
            errored = true;
        }
    }

    result = !errored;
    std::string codeStr;
    llvm::raw_string_ostream code (codeStr);
    module_->print (code, 0);
    return code.str ();
}

LLVMGenerator::~LLVMGenerator ()
{

}
}
}
}

