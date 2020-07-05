#include "BitcodeDialog.h"
#include "ui_BitcodeDialog.h"
#include "BitcodeHighlighter.h"

#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>

#include "lzstring.h"

#include <QDesktopServices>

struct LLVMGlobalContext
{
    llvm::LLVMContext Context;
    std::shared_ptr<llvm::Module> Module;

    LLVMGlobalContext() = default;

    LLVMGlobalContext(const LLVMGlobalContext &) = delete;

    bool Parse(const QByteArray& data)
    {
        // TODO: ModuleID comment goes missing
        llvm::StringRef sr(data.constData(), data.size());
        auto buf = llvm::MemoryBuffer::getMemBuffer(sr, "", false);
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

// TODO: do this properly https://github.com/Nanonid/rison
static QString risonencode(const QString& s)
{
    auto utf8 = s.toUtf8();
    QString r;
    for(const char ch : utf8)
    {
        if(ch == '\r')
            continue;
        if(ch == '\'')
        {
            r += "!'";
        }
        else if(ch == '!')
        {
            r += "!!";
        }
        else if(ch == ' ')
        {
            r += '+';
        }
        else if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '-' || ch == '.' || ch == '_' || ch == ':' || ch == '$' || ch == '@' || ch == '(' || ch == ')' || ch == '*' || ch == ',' || ch == '/')
        {
            r += ch;
        }
        else
        {
            r += QString().sprintf("%%%02X", (unsigned char)ch);
        }
    }
    return r;
}

void BitcodeDialog::on_buttonGodbolt_clicked()
{
    QString pattern = "g:!((g:!((g:!((h:codeEditor,i:(fontScale:14,j:2,lang:llvm,selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:'{}'),l:'5',n:'0',o:'LLVM+IR+source+%232',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:llctrunk,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'1',trim:'1'),fontScale:14,j:1,lang:llvm,libs:!(),options:'-O3',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:2),l:'5',n:'0',o:'llc+(trunk)+(Editor+%232,+Compiler+%231)+LLVM+IR',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4";
    auto text = ui->plainTextBitcode->toPlainText();
    text = risonencode(text);
    pattern = pattern.replace("{}", text);
    auto compressed = LZString::compressToBase64(pattern);
    auto encoded = QUrl::toPercentEncoding(compressed);
    auto url = QString("https://godbolt.org/#z:%1").arg(QString(encoded));
    QDesktopServices::openUrl(url);
}
