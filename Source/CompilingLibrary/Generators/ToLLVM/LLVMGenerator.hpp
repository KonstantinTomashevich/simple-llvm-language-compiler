#pragma once
#include <string>
#include <map>
#include <CompilingLibrary/Parser/AST.hpp>

#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

namespace Compiling
{
namespace Generators
{
namespace ToLLVM
{
class LLVMGenerator
{
protected:
    llvm::Module *module_;
    llvm::IRBuilder <> builder_;
    std::map <std::string, llvm::Value *> namedValues_;

    /// \brief Prints error to std::cerr and returns 0.
    llvm::Value *ErrorInCodeGeneration (const char *error);
    /// \brief Prints error to std::cerr and returns 0.
    llvm::Function *ErrorInFunctionCodeGeneration (const char *error);
    /// \brief Selects suitable generate function and returns generated value.
    llvm::Value *GenerateValue (Parser::AST::Expression *value);
    /// \brief Generates LLVM IR from AST::Number expression.
    llvm::Value *GenerateNumber (Parser::AST::Number *number);
    /// \brief Generates LLVM IR from AST::Variable expression.
    llvm::Value *GenerateVariable (Parser::AST::Variable *variable);
    /// \brief Generates LLVM IR from AST::TwoArgumentsOperator expression.
    llvm::Value *GenerateTwoArgumentsOperator (Parser::AST::TwoArgumentsOperator *operatorAST);
    /// \brief Generates LLVM IR from AST::Call expression.
    llvm::Value *GenerateCall (Parser::AST::Call *call);
    /// \brief Generates LLVM IR  from AST::Prototype value.
    llvm::Function *GeneratePrototype (Parser::AST::Prototype *prototype);
    /// \brief Generates LLVM IR  from AST::Function value
    llvm::Function *GenerateFunction (Parser::AST::Function *functionAST);
public:
    LLVMGenerator ();
    std::string GenerateCode (Parser::AST::TreeNode *rootASTNode, char *moduleName, bool &result);
    virtual ~LLVMGenerator ();
};
}
}
}
