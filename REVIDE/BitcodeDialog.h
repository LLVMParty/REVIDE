#pragma once

#include <QDialog>
#include "Styled.h"

class BitcodeHighlighter;

namespace Ui
{
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

static const char* annotationTypeName[] = {
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

class BitcodeDialog : public QDialog, Styled<BitcodeDialog>
{
    Q_OBJECT

public:
    CSS_COLOR(keywordColor);
    CSS_COLOR(instructionColor);
    CSS_COLOR(globalVariableColor);
    CSS_COLOR(localVariableColor);
    CSS_COLOR(constantColor);
    CSS_COLOR(integerTypeColor);
    CSS_COLOR(commentColor);
    CSS_COLOR(metadataColor);
    CSS_COLOR(functionColor);

public:
    explicit BitcodeDialog(QWidget* parent = nullptr);
    ~BitcodeDialog();
    bool load(const QString& type, const QByteArray& data, QString& errorMessage);

protected:
    void changeEvent(QEvent* event) override;

private slots:
    void on_buttonGodbolt_clicked();
    void on_buttonHelp_clicked();
    void on_plainTextBitcode_cursorPositionChanged();

private:
    Ui::BitcodeDialog* ui = nullptr;
    LLVMGlobalContext* mContext = nullptr;
    BitcodeHighlighter* mHighlighter = nullptr;
    QVector<AnnotatedLine> mAnnotatedLines;
    QString mErrorMessage = "index out of bounds";
    int mErrorLine = -1, mErrorColumn = -1;
};

