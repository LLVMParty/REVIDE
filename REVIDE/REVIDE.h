// Copy this header (together with httplib.h) to your project
// Then you can do REVIDE::Dump(Module) to dump an LLVM module

#pragma once

#include "httplib.h"

#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>

namespace REVIDE
{
  inline void Dump(const llvm::Module& Module, const std::string& title = std::string())
  {
    std::string str;
    std::error_code err;
    llvm::raw_string_ostream rso(str);
    Module.print(rso, nullptr);

    httplib::Client client("localhost", 1867);
    std::string path("/llvm?type=module&title=");
    path += httplib::detail::encode_url(title);
    str = httplib::detail::base64_encode(str);
    client.Post(path.c_str(), str, "text/plain");
  }

  inline void Dump(const llvm::Module* Module, const std::string& title = std::string())
  {
    return Dump(*Module, title);
  }

  inline void Dump(const std::shared_ptr<llvm::Module>& Module, const std::string& title = std::string())
  {
    return Dump(Module.get(), title);
  }

  inline void Dump(const std::unique_ptr<llvm::Module>& Module, const std::string& title = std::string())
  {
    return Dump(Module.get(), title);
  }
}
