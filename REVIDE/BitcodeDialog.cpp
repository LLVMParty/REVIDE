#include "BitcodeDialog.h"
#include "ui_BitcodeDialog.h"
#include "BitcodeHighlighter.h"
#include "FunctionDialog.h"
#include "DocumentationDialog.h"
#include "GraphDialog.h"
#include "QtHelpers.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <llvm/IR/CFG.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FormattedStream.h>

#include "lzstring.h"
#include <sstream>
#include <unordered_map>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextCursor>

static std::unordered_map<std::string, QString> instructionDocumentation;

struct LineAnnotationWriter : llvm::AssemblyAnnotationWriter
{
    /// emitFunctionAnnot - This may be implemented to emit a string right before
    /// the start of a function.
    void emitFunctionAnnot(const llvm::Function* F,
        llvm::formatted_raw_ostream& OS) override
    {
        OS << "; META:Function:" << F << "\n";
    }

    /// emitBasicBlockStartAnnot - This may be implemented to emit a string right
    /// after the basic block label, but before the first instruction in the
    /// block.
    void emitBasicBlockStartAnnot(const llvm::BasicBlock* BB,
        llvm::formatted_raw_ostream& OS) override
    {
        OS << "; META:BasicBlockStart:" << BB << "\n";
    }

    /// emitBasicBlockEndAnnot - This may be implemented to emit a string right
    /// after the basic block.
    void emitBasicBlockEndAnnot(const llvm::BasicBlock* BB,
        llvm::formatted_raw_ostream& OS) override
    {
        OS << "; META:BasicBlockEnd:" << BB << "\n";
    }

    /// emitInstructionAnnot - This may be implemented to emit a string right
    /// before an instruction is emitted.
    void emitInstructionAnnot(const llvm::Instruction* I,
        llvm::formatted_raw_ostream& OS) override
    {
        OS << "; META:Instruction:" << I << "\n";
    }

    /// printInfoComment - This may be implemented to emit a comment to the
    /// right of an instruction or global value.
    void printInfoComment(const llvm::Value& V,
        llvm::formatted_raw_ostream& OS) override
    {
    }
};

// Taken from WhitePeacock
struct LLVMGlobalContext
{
    llvm::LLVMContext Context;
    std::shared_ptr<llvm::Module> Module;
    std::vector<llvm::Function*> Functions;

    LLVMGlobalContext() = default;

    LLVMGlobalContext(const LLVMGlobalContext&) = delete;

    bool Parse(const QByteArray& data, QString& errorMessage, int& errorLine, int& errorColumn)
    {
        // TODO: ModuleID comment goes missing

        llvm::StringRef sr(data.constData(), data.size());
        auto buf = llvm::MemoryBuffer::getMemBuffer(sr, "", false);
        llvm::SMDiagnostic Err;
        auto irModule = parseIR(*buf, Err, Context);
        if (!irModule)
        {
            std::stringstream ss;
            errorLine = Err.getLineNo();
            if (errorLine != -1)
            {
                ss << "line: " << errorLine;
                errorColumn = Err.getColumnNo();
                if (errorColumn != -1)
                {
                    ss << ", column: " << errorColumn + 1;
                }
            }
            ss << ", message: " << Err.getMessage().str();
            ss << ", line contents: '" << Err.getLineContents().str() << "'";
            errorMessage = QString::fromStdString(ss.str()).trimmed();
            return false;
        }
        errorMessage.clear();
        Module = std::move(irModule);
        Functions.clear();
        for (auto& function : Module->functions())
            Functions.push_back(&function);
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
            if (line.startsWith("; META:"))
            {
                QStringList s = line.split(':');
                QString type = s[1];
                QString ptr = s[2];
                if (ptr.startsWith("0x"))
                    ptr = ptr.right(ptr.length() - 2);
                auto x = ptr.toULongLong(nullptr, 16);
                annotation.ptr = (void*)x;
                annotation.line = annotatedLines.length();
                if (type == "Function")
                {
                    annotation.type = AnnotationType::Function;
                }
                else if (type == "BasicBlockStart")
                {
                    annotation.type = AnnotationType::BasicBlockStart;
                    annotatedLines.back().annotation = annotation;
                }
                else if (type == "BasicBlockEnd")
                {
                    annotation.type = AnnotationType::BasicBlockEnd;
                }
                else if (type == "Instruction")
                {
                    annotation.type = AnnotationType::Instruction;
                }
            }
            else
            {
                annotatedLines.push_back({ line, annotation });
                if (annotation.type == AnnotationType::BasicBlockEnd)
                {
                    annotation = Annotation();
                }
            }
            line.clear();
        };
        for (size_t i = 0; i < str.length(); i++)
        {
            char ch = str[i];
            if (ch == '\r')
                continue;
            if (ch == '\n')
                flushLine();
            else
                line += ch;
        }
        if (!line.isEmpty())
            flushLine();
        return annotatedLines;
    }
};

BitcodeDialog::BitcodeDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::BitcodeDialog)
    , mContext(new LLVMGlobalContext())
{
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    setWindowFlag(Qt::WindowMinimizeButtonHint, true);
    ui->setupUi(this);
    qtRestoreGeometry(this);

    if (instructionDocumentation.empty())
    {
        QFile jsonFile(":/documentation/LLVM.json");
        if (!jsonFile.open(QFile::ReadOnly))
        {
            QMessageBox::critical(parent, tr("Error"), tr("Failed to load LLVM documentation"));
            return;
        }
        auto json = QJsonDocument::fromJson(jsonFile.readAll()).object();
        for (const auto& key : json.keys())
            instructionDocumentation[key.toStdString()] = json.value(key).toString();
    }

    mHighlighter = new BitcodeHighlighter(this, ui->plainTextBitcode->document());

    mFunctionDialog = new FunctionDialog(this);
    mFunctionDialog->show();
    connect(mFunctionDialog, &FunctionDialog::functionClicked, [this](int index)
        {
            mContext->Module->getFunctionList();
            QString name = mContext->Functions[index]->getName().str().c_str();
            auto line = mFunctionLineMap[index];
            auto block = ui->plainTextBitcode->document()->findBlockByNumber(line);
            ui->plainTextBitcode->moveCursor(QTextCursor::End);
            ui->plainTextBitcode->setTextCursor(QTextCursor(block.previous()));
            ui->plainTextBitcode->setTextCursor(QTextCursor(block));
        });

    mDocumentationDialog = new DocumentationDialog(this);
    mDocumentationDialog->show();

    mGraphDialog = new GraphDialog(this);
    mGraphDialog->show();
    connect(mGraphDialog->graphView(), &GenericGraphView::blockSelectionChanged, [this](ut64 blockId)
        {
            auto itr = mBlockIdToBlock.find(blockId);
            if(itr != mBlockIdToBlock.end())
            {
                qDebug() << "blockSelectionChanged" << blockId;
                auto line = mBlockLineMap.at(itr->second);
                auto block = ui->plainTextBitcode->document()->findBlockByNumber(line);
                // Attempt to center the start of the block in the view
                ui->plainTextBitcode->moveCursor(QTextCursor::End);
                ui->plainTextBitcode->setTextCursor(QTextCursor(block));
                auto cursor = ui->plainTextBitcode->textCursor();
                cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 10);
                ui->plainTextBitcode->setTextCursor(cursor);
                ui->plainTextBitcode->setTextCursor(QTextCursor(block));
                // TODO: prevent the selection?
            }
            else
            {
                QMessageBox::information(this, tr("Error"), tr("Unknown block id %1").arg(blockId));
            }
        });
}

BitcodeDialog::~BitcodeDialog()
{
    delete ui;
    delete mContext;
    delete mHighlighter;
}

bool BitcodeDialog::load(const QString& type, const QByteArray& data, QString& errorMessage)
{
    if (type == "module")
    {
        if (!mContext->Parse(data, errorMessage, mErrorLine, mErrorColumn))
        {
            mErrorMessage = errorMessage;
            ui->plainTextBitcode->setErrorLine(mErrorLine);
            ui->plainTextBitcode->setPlainText(data);
            auto cursor = ui->plainTextBitcode->textCursor();
            cursor.clearSelection();
            cursor.setPosition(ui->plainTextBitcode->document()->findBlockByLineNumber(mErrorLine - 1).position() + mErrorColumn);
            ui->plainTextBitcode->setTextCursor(cursor);
            return false;
        }
        mAnnotatedLines = mContext->Dump();
        QString text;
        for (const auto& annotatedLine : mAnnotatedLines)
        {
            auto line = annotatedLine.annotation.line;
            text += annotatedLine.line + "\n";
            switch (annotatedLine.annotation.type)
            {
            case AnnotationType::Function:
            {
                if (!mFunctionLineMap.isEmpty() && mFunctionLineMap.back() == line)
                    mFunctionLineMap.back() = line;
                else
                    mFunctionLineMap.push_back(annotatedLine.annotation.line);
            }
            break;

            case AnnotationType::BasicBlockStart:
            {
                auto basicBlock = (llvm::BasicBlock*)annotatedLine.annotation.ptr;
                mBlockLineMap.emplace(basicBlock, line - 1);
            }
            break;

            default:
                break;
            }
        }
        text.chop(1); // remove the last \n
        ui->plainTextBitcode->clear();
        // auto cursor = ui->plainTextBitcode->textCursor();
        // cursor.beginEditBlock();
        // cursor.insertBlock();
        // cursor.insertText(text);
        // cursor.endEditBlock();
        ui->plainTextBitcode->setPlainText(text);
        // ui->plainTextBitcode->appendPlainText(text);
        QStringList functionList;
        functionList.reserve(mContext->Functions.size());
        for (const auto& function : mContext->Functions)
            functionList << function->getName().str().c_str();
        mFunctionDialog->setFunctionList(functionList);
        qDebug() << "blockCount" << ui->plainTextBitcode->blockCount();
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
    for (const char ch : utf8)
    {
        if (ch == '\r')
            continue;
        if (ch == '\'')
        {
            r += "!'";
        }
        else if (ch == '!')
        {
            r += "!!";
        }
        else if (ch == ' ')
        {
            r += '+';
        }
        else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '-' || ch == '.' || ch == '_' || ch == ':' || ch == '$' || ch == '@' || ch == '(' || ch == ')' || ch == '*' || ch == ',' || ch == '/')
        {
            r += ch;
        }
        else
        {
            r += QString("%%%1").arg((unsigned char)ch, 10, QChar('0'));
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

void BitcodeDialog::on_buttonHelp_clicked()
{
    QDesktopServices::openUrl(QUrl("https://llvm.org/docs/LangRef.html#abstract"));
    mDocumentationDialog->show();
}

void BitcodeDialog::on_plainTextBitcode_cursorPositionChanged()
{
    auto line = ui->plainTextBitcode->textCursor().block().firstLineNumber();
    mDocumentationDialog->setHtml("");
    QString info;
    if (line >= mAnnotatedLines.length())
    {
        info = mErrorMessage;
    }
    else
    {
        const llvm::Function* selectedFn = nullptr;
        const llvm::BasicBlock* selectedBB = nullptr;

        const Annotation& annotation = mAnnotatedLines[line].annotation;
        auto typeName = annotationTypeName[(int)annotation.type];
        QString info2;
        switch (annotation.type)
        {
        case AnnotationType::Nothing:
        {
            // TODO: get closest?
            info2 = "";
        }
        break;

        case AnnotationType::Function:
        {
            auto function = (llvm::Function*)annotation.ptr;
            info2 = QString(", name: %1").arg(function->getName().str().c_str());

            selectedFn = function;
            if (!function->empty())
                selectedBB = &function->getEntryBlock();
        }
        break;

        case AnnotationType::BasicBlockEnd:
        case AnnotationType::BasicBlockStart:
        {
            auto basicBlock = (llvm::BasicBlock*)annotation.ptr;
            info2 = QString(", name: %1").arg(basicBlock->getName().str().c_str());
            selectedBB = basicBlock;
        }
        break;

        case AnnotationType::Instruction:
        {
            auto instruction = (llvm::Instruction*)annotation.ptr;
            auto opcode = instruction->getOpcodeName();
            info2 = QString(", opcode: %1").arg(opcode);
            // auto x = instruction->metadata
            auto metadata = instruction->getMetadata("UNKNOWN");
            if (metadata)
            {
                std::string ss;
                auto x = llvm::raw_string_ostream(ss);
                metadata->print(x, instruction->getModule(), true);
                qDebug() << ss.c_str();
                // metadata->dump();
                // instruction->getMetadata()
            }

            auto itr = instructionDocumentation.find(opcode);
            if (itr != instructionDocumentation.end())
                mDocumentationDialog->setHtml(itr->second);

            selectedBB = instruction->getParent();
        }
        break;
        }

        if (selectedBB != nullptr && selectedFn == nullptr)
            selectedFn = selectedBB->getParent();

        // Update the graph if a valid function is selected
        if (selectedFn != nullptr && !selectedFn->empty())
        {
            auto foundGraph = mFunctionGraphs.find(selectedFn);
            if (foundGraph == mFunctionGraphs.end())
            {
                // Create the graph if it doesn't exist yet
                // TODO: this isn't very performance friendly, needs to be moved to a thread pool
                GenericGraph graph(mCurrentGraphId++);

                for (const auto& BB : *selectedFn)
                {
                    auto name = QString::fromStdString(BB.getName().str());
                    auto id = getBlockId(&BB);
                    graph.addNode(id, name);
                    // https://stackoverflow.com/a/59933151/1806760
                    for (auto pred : llvm::predecessors(&BB))
                    {
                        graph.addEdge(getBlockId(pred), id);
                    }
                }

                foundGraph = mFunctionGraphs.emplace(selectedFn, std::move(graph)).first;
            }

            mGraphDialog->graphView()->setGraph(foundGraph->second);

            if (selectedBB != nullptr)
            {
                mGraphDialog->graphView()->selectBlockWithId(getBlockId(selectedBB));
            }

            mGraphDialog->show();
        }
        else
        {
            mGraphDialog->hide();
        }

        info = QString("line %1, type: %2%3").arg(line).arg(typeName).arg(info2);
    }
    ui->lineEditStatus->setText(info);
}

void BitcodeDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::StyleChange)
    {
        if (mHighlighter)
        {
            ensurePolished();
            mHighlighter->refreshColors(this);
            mHighlighter->setDocument(nullptr);
            mHighlighter->setDocument(ui->plainTextBitcode->document());
        }
    }
    QDialog::changeEvent(event);
}

void BitcodeDialog::closeEvent(QCloseEvent* event)
{
    qtSaveGeometry(this);
    mFunctionDialog->close();
    mDocumentationDialog->close();
    mGraphDialog->close();
    return QDialog::closeEvent(event);
}

ut64 BitcodeDialog::getBlockId(const llvm::BasicBlock* block)
{
    if (block == nullptr)
        throw std::logic_error("No ID for null block allowed!");

    auto itr = mBlockToBlockId.find(block);
    if (itr == mBlockToBlockId.end())
    {
        auto id = mCurrentBlockId++;
        itr = mBlockToBlockId.emplace(block, id).first;
        mBlockIdToBlock[id] = block;
    }
    return itr->second;
}
