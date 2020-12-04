#pragma once

#include <QDialog>
#include "Styled.h"

class BitcodeHighlighter;
class FunctionDialog;

namespace Ui
{
class VTILDialog;
}

class VTILDialog : public QDialog, Styled<VTILDialog>
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
    explicit VTILDialog(QWidget* parent = nullptr);
    ~VTILDialog();
    bool load(const QByteArray& data, QString& errorMessage);

protected:
    void changeEvent(QEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::VTILDialog* ui = nullptr;
    FunctionDialog* mFunctionDialog;
};

