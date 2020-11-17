// Copy this header (together with httplib.h) to your project
// Then you can do REVIDE::Dump(Module) to dump an LLVM module

#pragma once

#include "httplib.h"

#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Utils/Cloning.h>

namespace REVIDE
{
inline void Dump(llvm::Module& Module, const std::string& title = std::string())
{

#if 0
    // TODO: CloneModule appears to be crashing
    //auto clone = llvm::CloneModule(Module);
    // TODO: clone the module
    // TODO: print errors in the user's console about these things

    // for functions without a return, insert a return
    for (auto& F : Module)
    {
        llvm::BasicBlock* lastBB = nullptr;
        bool hasReturn = [&F, &lastBB]() {
            for (auto& BB : F)
            {
                lastBB = &BB;
                for (auto& I : BB)
                {
                    if (I.getOpcode() == llvm::Instruction::Ret)
                    {
                        return true;
                    }
                }
            }
            return false;
        }();

        if (!hasReturn && lastBB)
        {
            fprintf(stderr, "Function without return: %s\n", F.getName().str().c_str());
            llvm::ReturnInst::Create(Module.getContext(), lastBB);
        }
    }

    // for empty basic blocks, insert a dummy instruction
    for (auto& F : Module)
    {
        for (auto& BB : F)
        {
            if (BB.empty())
            {
                llvm::BinaryOperator::CreateAdd(
                    llvm::ConstantInt::get(llvm::Type::getInt64Ty(Module.getContext()), 0),
                    llvm::ConstantInt::get(llvm::Type::getInt64Ty(Module.getContext()), 0),
                    llvm::Twine("nop"),
                    &BB);
            }
        }
    }
#endif

    std::string str;
    std::error_code err;
    llvm::raw_string_ostream rso(str);
    Module.print(rso, nullptr, false, true);

    httplib::Client client("localhost", 13337);
    std::string path("/llvm?type=module&title=");
    path += httplib::detail::encode_url(title);
    str = httplib::detail::base64_encode(str);
    client.Post(path.c_str(), str, "text/plain");
} // namespace REVIDE

inline void Dump(llvm::Module* Module, const std::string& title = std::string())
{
    return Dump(*Module, title);
}

inline void Dump(std::shared_ptr<llvm::Module>& Module, const std::string& title = std::string())
{
    return Dump(Module.get(), title);
}

inline void Dump(std::unique_ptr<llvm::Module>& Module, const std::string& title = std::string())
{
    return Dump(Module.get(), title);
}
} // namespace REVIDE
