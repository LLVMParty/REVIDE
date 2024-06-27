#include "BitcodeDialog.h"
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
#include "DockAreaWidget.h"

#include <QLayout>
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
        if(llvm::dyn_cast<llvm::Instruction>(&V) == nullptr) {
            OS << "\n; META:Global:" << &V;
        }
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
                ss << ", message: " << Err.getMessage().str();
                ss << ", line contents: '" << Err.getLineContents().str() << "'";
            }
            else
            {
                ss << Err.getMessage().str();
            }
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
        Annotation nextAnnotation;
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
                Annotation annotation;
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
                else if (type == "Global")
                {
                    annotation.type = AnnotationType::Global;
                }

                if(annotation.type == AnnotationType::Global)
                {
                    // Global annotations are emitted on the next line
                    if(!annotatedLines.empty())
                    {
                        annotation.line--;
                        annotatedLines.back().annotation = annotation;
                    }
                }
                else
                {
                    nextAnnotation = annotation;
                }
            }
            else
            {
                annotatedLines.push_back({ line, nextAnnotation });
                if (nextAnnotation.type == AnnotationType::BasicBlockEnd)
                {
                    nextAnnotation = Annotation();
                }
                else if(nextAnnotation.type == AnnotationType::Function)
                {
                    auto function = (llvm::Function*)nextAnnotation.ptr;
                    if(line.startsWith("declare") || line.endsWith('{'))
                    {
                        nextAnnotation = Annotation();
                    }
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
    : ads::CDockManager(parent)
    , mContext(new LLVMGlobalContext())
{
    auto codeWidget = new QWidget();
    codeWidget->setWindowTitle(tr("Code"));
    mPlainTextBitcode = new CodeEditor(codeWidget);
    mPlainTextBitcode->setContextMenuPolicy(Qt::CustomContextMenu);
    mPlainTextBitcode->setHackedReadonly(true);
    connect(mPlainTextBitcode, &CodeEditor::cursorPositionChanged, this, &BitcodeDialog::bitcodeCursorPositionChangedSlot);
    connect(mPlainTextBitcode, &CodeEditor::customContextMenuRequested, this, &BitcodeDialog::bitcodeContextMenuSlot);

    setupMenu();

    mLineEditStatus = new QLineEdit(codeWidget);
    mLineEditStatus->setReadOnly(true);

    mButtonGodbolt = new QPushButton(tr("Godbolt"), codeWidget);
    connect(mButtonGodbolt, &QPushButton::clicked, this, &BitcodeDialog::godboltClickedSlot);

    mButtonHelp = new QPushButton(tr("Help"), codeWidget);
    connect(mButtonHelp, &QPushButton::clicked, this, &BitcodeDialog::helpClickedSlot);

    auto horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(mLineEditStatus);
    horizontalLayout->addWidget(mButtonGodbolt);
    horizontalLayout->addWidget(mButtonHelp);

    auto verticalLayout = new QVBoxLayout();
    verticalLayout->addWidget(mPlainTextBitcode);
    verticalLayout->addLayout(horizontalLayout);
    codeWidget->setLayout(verticalLayout);

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

    mHighlighter = new BitcodeHighlighter(this, mPlainTextBitcode->document());

    mFunctionDialog = new FunctionDialog(this);
    //mFunctionDialog->show();
    connect(mFunctionDialog, &FunctionDialog::functionClicked, [this](int index)
        {
            auto function = mContext->Functions[index];
            auto line = mFunctionLineMap[function];
            if(line + 1 < mAnnotatedLines.size() && mAnnotatedLines[line + 1].annotation.ptr == function)
            {
                line++;
            }
            gotoLine(line, false);
        });

    mDocumentationDialog = new DocumentationDialog(this);
    //mDocumentationDialog->show();

    mGraphDialog = new GraphDialog(this);
    //mGraphDialog->show();
    connect(mGraphDialog->graphView(), &GenericGraphView::blockSelectionChanged, [this](ut64 blockId)
        {
            auto itr = mBlockIdToBlock.find(blockId);
            if (itr != mBlockIdToBlock.end())
            {
                qDebug() << "blockSelectionChanged" << blockId;
                auto line = mBlockLineMap.at(itr->second);
                gotoLine(line, true);
            }
            else
            {
                QMessageBox::information(this, tr("Error"), tr("Unknown block id %1").arg(blockId));
            }
        });

    setConfigFlag(ads::CDockManager::DockAreaHasCloseButton, false);
    setConfigFlag(ads::CDockManager::DockAreaHasTabsMenuButton, false);

    auto dockHelper = [this](ads::DockWidgetArea area, QWidget* widget, ads::CDockWidget* inside = nullptr)
    {
        auto dockWidget = new ads::CDockWidget(widget->windowTitle(), this);
        dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, false);
        dockWidget->setWidget(widget);
        addDockWidget(area, dockWidget, inside ? inside->dockAreaWidget() : nullptr);
        return dockWidget;
    };

    dockHelper(ads::CenterDockWidgetArea, codeWidget);
    dockHelper(ads::RightDockWidgetArea, mDocumentationDialog);
    auto functionDockWidget = dockHelper(ads::LeftDockWidgetArea, mFunctionDialog);
    dockHelper(ads::BottomDockWidgetArea, mGraphDialog, functionDockWidget);

    qtRestoreState(this);
}

BitcodeDialog::~BitcodeDialog()
{
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
            mPlainTextBitcode->setErrorLine(mErrorLine);
            if (data.length() > 4 && data[0] == 'B' && data[1] == 'C' && data[2] == 0xC0 && data[3] == 0xDE)
            {
                mPlainTextBitcode->setPlainText(errorMessage);
            }
            else
            {
                mPlainTextBitcode->setPlainText(data);
            }
            auto cursor = mPlainTextBitcode->textCursor();
            cursor.clearSelection();
            cursor.setPosition(mPlainTextBitcode->document()->findBlockByLineNumber(mErrorLine - 1).position() + mErrorColumn);
            mPlainTextBitcode->setTextCursor(cursor);
            return false;
        }
        mAnnotatedLines = mContext->Dump();
        QString text;
        mFunctionLineMap.clear();
        mBlockLineMap.clear();
        mFunctionGraphs.clear();
        mBlockIdToBlock.clear();
        mBlockToBlockId.clear();
        mSelectedValue = nullptr;
        for (const auto& annotatedLine : mAnnotatedLines)
        {
            auto line = annotatedLine.annotation.line;
            text += annotatedLine.line + "\n";
            switch (annotatedLine.annotation.type)
            {
            case AnnotationType::Function:
            {
                auto function = (llvm::Function*)annotatedLine.annotation.ptr;
                mFunctionLineMap.emplace(function, annotatedLine.annotation.line);
            }
            break;

            case AnnotationType::BasicBlockStart:
            {
                auto basicBlock = (llvm::BasicBlock*)annotatedLine.annotation.ptr;
                if(mBlockLineMap.emplace(basicBlock, line - 1).second)
                {
                    auto label = annotatedLine.line.split(':')[0];
                    if (basicBlock == &basicBlock->getParent()->getEntryBlock())
                        label = "entry";
                    mBlockLabelMap.emplace(basicBlock, label);
                }
            }
            break;

            default:
                break;
            }
        }
        text.chop(1); // remove the last \n
        mPlainTextBitcode->clear();
        // auto cursor = mPlainTextBitcode->textCursor();
        // cursor.beginEditBlock();
        // cursor.insertBlock();
        // cursor.insertText(text);
        // cursor.endEditBlock();
        mPlainTextBitcode->setPlainText(text);
        // mPlainTextBitcode->appendPlainText(text);
        QStringList functionList;
        functionList.reserve(mContext->Functions.size());
        for (const auto& function : mContext->Functions)
            functionList << function->getName().str().c_str();
        mFunctionDialog->setFunctionList(functionList);
        qDebug() << "blockCount" << mPlainTextBitcode->blockCount();
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

void BitcodeDialog::godboltClickedSlot()
{
    QString pattern = "g:!((g:!((g:!((h:codeEditor,i:(fontScale:14,j:2,lang:llvm,selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:'{}'),l:'5',n:'0',o:'LLVM+IR+source+%232',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:llctrunk,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'1',trim:'1'),fontScale:14,j:1,lang:llvm,libs:!(),options:'-O3',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:2),l:'5',n:'0',o:'llc+(trunk)+(Editor+%232,+Compiler+%231)+LLVM+IR',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4";
    auto text = mPlainTextBitcode->toPlainText();
    text = risonencode(text);
    pattern = pattern.replace("{}", text);
    auto compressed = LZString::compressToBase64(pattern);
    auto encoded = QUrl::toPercentEncoding(compressed);
    auto url = QString("https://godbolt.org/#z:%1").arg(QString(encoded));
    QDesktopServices::openUrl(url);
}

void BitcodeDialog::helpClickedSlot()
{
    QDesktopServices::openUrl(QUrl("https://llvm.org/docs/LangRef.html#abstract"));
    //mDocumentationDialog->show();
}

struct Token
{
    QString text;
    bool isVariable = false;
};

static std::vector<Token> tokenizeLlvm(const QString& text)
{
    // TODO: extremely hacky
    std::vector<Token> tokens;
    Token cur;
    auto flushToken = [&]()
    {
        if(!cur.text.isEmpty())
            tokens.push_back(cur);
        cur = Token();
    };
    bool isQuoted = false;
    for(int i = 0; i < text.length(); i++)
    {
        auto ch = text[i];
        if(isQuoted)
        {
            if(ch == '\"')
            {
                isQuoted = false;
                cur.text.push_back(ch);
                flushToken();
            }
            else
            {
                cur.text.push_back(ch);
            }
        }
        else if(cur.isVariable)
        {
            if(ch.isLetterOrNumber() || ch == '-' || ch =='.' || ch == '$')
            {
                cur.text.push_back(ch);
            }
            else
            {
                flushToken();
                cur.text.push_back(ch);
                if(ch == ' ' || ch == ',' || ch == '(' || ch == ')')
                {
                    flushToken();
                }
            }
        }
        else
        {
            if(ch == '%' || ch == '@')
            {
                flushToken();
                cur.isVariable = true;
                if(i + 1 < text.length() && text[i + 1] == '\"')
                {
                    isQuoted = true;
                    cur.text.push_back(ch);
                    cur.text.push_back('\"');
                    i++;
                }
                else
                {
                    cur.text.push_back(ch);
                }
            }
            else if(ch == '\"')
            {
                isQuoted = true;
                flushToken();
                cur.text.push_back(ch);
            }
            else if(ch == ' ' || ch == ',' || ch == '(' || ch == ')')
            {
                flushToken();
                cur.text.push_back(ch);
                flushToken();
            }
            else
            {
                cur.text.push_back(ch);
            }
        }
    }
    flushToken();
    return tokens;
}

static llvm::Value* valueFromOperand(llvm::Value* op)
{
    if(auto mdv = llvm::dyn_cast<llvm::MetadataAsValue>(op))
    {
        auto md = mdv->getMetadata();
        if(auto vmd = llvm::dyn_cast<llvm::ValueAsMetadata>(md))
        {
            return vmd->getValue();
        }
        else if(auto cmd = llvm::dyn_cast<llvm::ConstantAsMetadata>(md))
        {
            return cmd->getValue();
        }
        else
        {
            llvm::errs() << "UNKNOWN METADATA: " << *mdv << "\n";
            return nullptr;
        }
    }
    else if(auto fn = llvm::dyn_cast<llvm::Function>(op))
    {
        return op;
    }
    else if(auto inst = llvm::dyn_cast<llvm::Instruction>(op))
    {
        return op;
    }
    else if(auto constant = llvm::dyn_cast<llvm::Constant>(op))
    {
        return op;
    }
    else if(auto bbLabel = llvm::dyn_cast<llvm::BasicBlock>(op))
    {
        return op;
    }
    else if(auto arg = llvm::dyn_cast<llvm::Argument>(op))
    {
        return op;
    }
    else
    {
        llvm::errs() << "UNKNOWN " << *op << "\ntype:";
        op->getType()->print(llvm::errs());
        llvm::errs() << "\n";
        return nullptr;
    }
}

static QString valueName(const llvm::Value* value)
{
    std::string str;
    llvm::raw_string_ostream rso(str);
    value->printAsOperand(rso, false);
    return QString::fromStdString(str);
}

static llvm::Value* findSelectedValue(const QString& selectedName, llvm::Instruction* instruction)
{
    if(selectedName == valueName(instruction))
    {
        return instruction;
    }

    for(unsigned int i = 0; i < instruction->getNumOperands(); i++)
    {
        auto op = instruction->getOperand(i);
        auto opValue = valueFromOperand(op);
        if(opValue != nullptr)
        {
            auto opName = valueName(opValue);
            if(selectedName == valueName(opValue))
            {
                return opValue;
            }
        }
    }

    return nullptr;
}

void BitcodeDialog::bitcodeCursorPositionChangedSlot()
{
    if (mIgnoreCursorMove)
        return;

    mSelectedValue = nullptr;
    auto cursor = mPlainTextBitcode->textCursor();
    auto line = cursor.block().firstLineNumber();
    auto column = cursor.columnNumber();
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

            if(annotation.line == line)
            {
                // Single line instruction
                auto tokens = tokenizeLlvm(mAnnotatedLines[line].line);

                /*qDebug() << "tokens {";
                for(const auto& token : tokens)
                {
                    qDebug() << token.text;
                }
                qDebug() << "}";*/

                Token* selectedToken = nullptr;
                for(size_t i = 0, start = 0; i < tokens.size(); i++)
                {
                    auto token = &tokens[i];
                    auto length = token->text.length();
                    if(column - 1 >= start && column - 1 < start + length)
                    {
                        selectedToken = token;
                        break;
                    }
                    start += length;
                }

                if(selectedToken != nullptr && selectedToken->isVariable)
                {
                    mPlainTextBitcode->setTokenHighlights(selectedToken->text, {});
                    if(auto selectedValue = findSelectedValue(selectedToken->text, instruction))
                    {
                        //llvm::errs() << "selected: " << *selectedValue << "\n";
                        info2 += QString(", selected: '%1'").arg(selectedToken->text);
                        mSelectedValue = selectedValue;
                    }
                }
            }
            else
            {
                // TODO: support multi-line instructions
                info2 += QString(", multiline!");
            }

            auto itr = instructionDocumentation.find(opcode);
            if (itr != instructionDocumentation.end())
                mDocumentationDialog->setHtml(itr->second);

            selectedBB = instruction->getParent();
        }
        break;

        case AnnotationType::Global:
        {
            auto value = (llvm::Value*)annotation.ptr;
            info2 = QString(", name: %1").arg(value->getName().str().c_str());
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
                    if (name.isEmpty())
                        name = mBlockLabelMap.at(&BB);
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

            //mGraphDialog->show();
        }
        else
        {
            qDebug() << "cursor changed" << line;
            //mGraphDialog->hide();
        }

        info = QString("line %1, col %2, type: %3%4").arg(line).arg(column).arg(typeName, info2);
    }
    mLineEditStatus->setText(info);
}

void BitcodeDialog::bitcodeContextMenuSlot(const QPoint& pos)
{
    auto menu = new QMenu(this);
    if(mSelectedValue != nullptr)
    {
        menu->addAction(mFollowValue);
    }
    if(!menu->actions().empty())
    {
        auto gpos = mPlainTextBitcode->mapToGlobal(pos);
        gpos.setX(gpos.x() + mPlainTextBitcode->lineNumberAreaWidth());
        menu->popup(gpos);
    }
}

void BitcodeDialog::followValueSlot()
{
    auto sel = mSelectedValue;
    if(sel == nullptr)
    {
        return;
    }

    if(auto argument = llvm::dyn_cast<llvm::Argument>(sel))
    {
        qDebug() << "follow argument";
        auto function = argument->getParent();
        auto itr = mFunctionLineMap.find(function);
        if(itr != mFunctionLineMap.end())
        {
            // TODO: select the argument itself
            auto line = itr->second;
            if(line + 1 < mAnnotatedLines.size() && mAnnotatedLines[line + 1].annotation.ptr == function)
            {
                line++;
            }
            gotoLine(line, false);
        }
    }
    else if(auto function = llvm::dyn_cast<llvm::Function>(sel))
    {
        qDebug() << "follow function";
        auto itr = mFunctionLineMap.find(function);
        if(itr != mFunctionLineMap.end())
        {
            auto line = itr->second;
            if(line + 1 < mAnnotatedLines.size() && mAnnotatedLines[line + 1].annotation.ptr == function)
            {
                line++;
            }
            gotoLine(line, false);
        }
    }
    else if(auto basicBlock = llvm::dyn_cast<llvm::BasicBlock>(sel))
    {
        qDebug() << "follow basic block";
        auto itr = mBlockLineMap.find(basicBlock);
        if(itr != mBlockLineMap.end())
        {
            gotoLine(itr->second, true);
        }
    }
    else if(auto instruction = llvm::dyn_cast<llvm::Instruction>(sel))
    {
        qDebug() << "follow instruction";
        auto basicBlock = instruction->getParent();
        auto itr = mBlockLineMap.find(basicBlock);
        if(itr != mBlockLineMap.end())
        {
            for(auto line = itr->second; line < mAnnotatedLines.size(); line++)
            {
                const auto& annotation = mAnnotatedLines[line].annotation;
                if(annotation.type == AnnotationType::BasicBlockEnd || annotation.type == AnnotationType::Function)
                {
                    break;
                }

                if(annotation.type == AnnotationType::Instruction && annotation.ptr == instruction)
                {
                    gotoLine(line, true);
                    break;
                }
            }
        }
    }
    else if(auto global = llvm::dyn_cast<llvm::GlobalValue>(sel))
    {
        qDebug() << "follow global";
        for(auto line = 0; line < mAnnotatedLines.size(); line++)
        {
            const auto& annotation = mAnnotatedLines[line].annotation;
            if(annotation.type == AnnotationType::Function)
            {
                // Globals are declared before the functions
                break;
            }

            if(annotation.type == AnnotationType::Global && annotation.ptr == global)
            {
                gotoLine(line, false);
                break;
            }
        }
    }
    else
    {
        qDebug() << "follow UNKNOWN";
    }
}

void BitcodeDialog::setupMenu()
{
    mFollowValue = new QAction("Follow value", this);
    mFollowValue->setShortcutContext(Qt::WidgetShortcut);
    mFollowValue->setShortcut(QKeySequence("F"));
    mPlainTextBitcode->addAction(mFollowValue);
    connect(mFollowValue, &QAction::triggered, this, &BitcodeDialog::followValueSlot);
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
            mHighlighter->setDocument(mPlainTextBitcode->document());
        }
    }
    ads::CDockManager::changeEvent(event);
}

void BitcodeDialog::closeEvent(QCloseEvent* event)
{
    qtSaveState(this);
    //mFunctionDialog->close();
    //mDocumentationDialog->close();
    //mGraphDialog->close();
    return ads::CDockManager::closeEvent(event);
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

void BitcodeDialog::gotoLine(int line, bool centerInView)
{
    auto block = mPlainTextBitcode->document()->findBlockByNumber(line);
    if(!block.isValid())
        return;

    if(centerInView)
    {
        mIgnoreCursorMove = true;
        // Attempt to center the start of the block in the view
        mPlainTextBitcode->moveCursor(QTextCursor::End);
        mPlainTextBitcode->setTextCursor(QTextCursor(block));
        auto cursor = mPlainTextBitcode->textCursor();
        cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 10);
        mPlainTextBitcode->setTextCursor(cursor);
        mIgnoreCursorMove = false;
        mPlainTextBitcode->setTextCursor(QTextCursor(block));
    }
    else
    {
        mPlainTextBitcode->moveCursor(QTextCursor::End);
        mPlainTextBitcode->setTextCursor(QTextCursor(block.previous()));
        mPlainTextBitcode->setTextCursor(QTextCursor(block));
    }

    // TODO: prevent the selection?
}
