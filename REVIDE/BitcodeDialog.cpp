#include "BitcodeDialog.h"
#include "ui_BitcodeDialog.h"
#include "BitcodeHighlighter.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FormattedStream.h>

#include "lzstring.h"

#include <QDesktopServices>

struct LineAnnotationWriter : llvm::AssemblyAnnotationWriter {
    /// emitFunctionAnnot - This may be implemented to emit a string right before
    /// the start of a function.
    void emitFunctionAnnot(const llvm::Function *F,
                           llvm::formatted_raw_ostream &OS) override {
        OS << "; META:Function:" << F << "\n";
    }

    /// emitBasicBlockStartAnnot - This may be implemented to emit a string right
    /// after the basic block label, but before the first instruction in the
    /// block.
    void emitBasicBlockStartAnnot(const llvm::BasicBlock *BB,
                                  llvm::formatted_raw_ostream &OS) override {
        OS << "; META:BasicBlockStart:" << BB << "\n";
    }

    /// emitBasicBlockEndAnnot - This may be implemented to emit a string right
    /// after the basic block.
    void emitBasicBlockEndAnnot(const llvm::BasicBlock *BB,
                                llvm::formatted_raw_ostream &OS) override {
        OS << "; META:BasicBlockEnd:" << BB << "\n";
    }

    /// emitInstructionAnnot - This may be implemented to emit a string right
    /// before an instruction is emitted.
    void emitInstructionAnnot(const llvm::Instruction *I,
                              llvm::formatted_raw_ostream &OS) override {
        OS << "; META:Instruction:" << I << "\n";
    }

    /// printInfoComment - This may be implemented to emit a comment to the
    /// right of an instruction or global value.
    void printInfoComment(const llvm::Value &V,
                          llvm::formatted_raw_ostream &OS) override {
    }
};

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

    QVector<AnnotatedLine> Dump()
    {
        std::string str;
        llvm::raw_string_ostream rso(str);
        LineAnnotationWriter annotationWriter;
        Module->print(rso, &annotationWriter, true, true);
        QVector<AnnotatedLine> annotatedLines;
        QString line;
        Annotation annotation;
        auto flushLine = [&]()
        {
            if(line.startsWith("; META:"))
            {
                QStringList s = line.split(':');
                QString type = s[1];
                QString ptr = s[2];
                if(ptr.startsWith("0x"))
                    ptr = ptr.right(ptr.length() - 2);
                auto x = ptr.toULongLong(nullptr, 16);
                annotation.ptr = (void*)x;
                annotation.line = annotatedLines.length();
                if(type == "Function")
                {
                    annotation.type = AnnotationType::Function;
                }
                else if(type == "BasicBlockStart")
                {
                    annotation.type = AnnotationType::BasicBlockStart;
                    annotatedLines.back().annotation = annotation;
                }
                else if(type == "BasicBlockEnd")
                {
                    annotation.type = AnnotationType::BasicBlockEnd;
                }
                else if(type == "Instruction")
                {
                    annotation.type = AnnotationType::Instruction;
                }
            }
            else
            {
                annotatedLines.push_back({line, annotation});
                if(annotation.type == AnnotationType::BasicBlockEnd)
                {
                    annotation = Annotation();
                }
            }
            line.clear();
        };
        for (size_t i = 0; i < str.length(); i++)
        {
            char ch = str[i];
            if(ch == '\r')
                continue;
            if(ch == '\n')
                flushLine();
            else
                line += ch;
        }
        if(!line.isEmpty())
            flushLine();
        return annotatedLines;
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
        mAnnotatedLines = mContext->Dump();
        QString text;
        for(const auto& annotatedLine : mAnnotatedLines)
            text += annotatedLine.line + "\n";
        text.chop(1);
        ui->plainTextBitcode->setPlainText(text);
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

void BitcodeDialog::on_plainTextBitcode_cursorPositionChanged()
{
    auto line = ui->plainTextBitcode->textCursor().block().firstLineNumber();
    QString info;
    if(line >= mAnnotatedLines.length())
    {
        info = "index out of bounds";
    }
    else
    {
        const Annotation& annotation = mAnnotatedLines[line].annotation;
        auto typeName = annotationTypeName[(int)annotation.type];
        QString info2;
        switch(annotation.type)
        {
        case AnnotationType::Nothing:
        {
            info2 = "";
        }
        break;

        case AnnotationType::Function:
        {
            auto function = (llvm::Function*)annotation.ptr;
            info2 = QString(", name: %1").arg(function->getName().str().c_str());
        }
        break;

        case AnnotationType::BasicBlockEnd:
        case AnnotationType::BasicBlockStart:
        {
            auto basicBlock = (llvm::BasicBlock*)annotation.ptr;
            info2 = QString(", name: %1").arg(basicBlock->getName().str().c_str());
        }
        break;

        case AnnotationType::Instruction:
        {
            auto instruction = (llvm::Instruction*)annotation.ptr;
            info2 = QString(", opcode: %1").arg(instruction->getOpcodeName());
        }
        break;
        }

        info = QString("line %1, type: %2%3").arg(line).arg(typeName).arg(info2);
    }
    ui->lineEditStatus->setText(info);
}
