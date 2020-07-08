#pragma once

#include <QDialog>
#include <memory>

class BitcodeHighlighter;

namespace Ui {
class BitcodeDialog;
}

struct LLVMGlobalContext;

enum class AnnotationType
{
    Nothing,
    Function,
    BasicBlockStart,
    Instruction,
    BasicBlockEnd,
};

static const char* annotationTypeName[] =
{
    "Nothing",
    "Function",
    "BasicBlockStart",
    "Instruction",
    "BasicBlockEnd",
};

struct Annotation
{
    AnnotationType type = {};
    unsigned line = 0;
    const void* ptr = nullptr;
};

struct AnnotatedLine
{
    QString line;
    Annotation annotation;
};

class BitcodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BitcodeDialog(QWidget* parent = nullptr);
    ~BitcodeDialog();
    bool load(const QString& type, const QByteArray& data);

private slots:
    void on_buttonGodbolt_clicked();
    void on_plainTextBitcode_cursorPositionChanged();

private:
    Ui::BitcodeDialog* ui = nullptr;
    std::unique_ptr<LLVMGlobalContext> mContext;
    std::unique_ptr<BitcodeHighlighter> mHighlighter;
    QVector<AnnotatedLine> mAnnotatedLines;
};

