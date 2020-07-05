#include "BitcodeDialog.h"
#include "ui_BitcodeDialog.h"
#include "BitcodeHighlighter.h"

#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>

struct LLVMGlobalContext
{
    llvm::LLVMContext Context;
    std::shared_ptr<llvm::Module> Module;

    LLVMGlobalContext() = default;

    LLVMGlobalContext(const LLVMGlobalContext &) = delete;

    bool Parse(const QByteArray& data)
    {
        llvm::StringRef sr(data.constData(), data.size());
        auto buf=llvm::MemoryBuffer::getMemBuffer(sr, "", false);
        llvm::SMDiagnostic Err;
        auto module = parseIR(*buf, Err, this->Context);
        if (!module)
            return false;
        this->Module = std::move(module);
        return true;
    }

    // TODO: this should output to some structure
    QString Dump()
    {
        std::string str;
        std::error_code err;
        llvm::raw_string_ostream rso(str);
        Module->print(rso, nullptr);
        return QString::fromStdString(str);
    }
};

BitcodeDialog::BitcodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BitcodeDialog),
    mContext(std::make_unique<LLVMGlobalContext>())
{
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->setupUi(this);
    mHighlighter = std::make_unique<BitcodeHighlighter>(ui->plainTextBitcode->document());
}

BitcodeDialog::~BitcodeDialog()
{
    delete ui;
}

bool BitcodeDialog::load(const QString& type, const QByteArray& data)
{
    if(type == "module")
    {
        if(!mContext->Parse(data))
            return false;
        ui->plainTextBitcode->setPlainText(mContext->Dump());
        return true;
    }
    else
    {
        return false;
    }
}
