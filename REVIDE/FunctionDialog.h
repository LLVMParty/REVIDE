#pragma once

#include <QDialog>

#include "AbstractFunctionList.h"

namespace Ui
{
class FunctionDialog;
}

class FunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FunctionDialog(QWidget* parent = nullptr);
    ~FunctionDialog();
    void setFunctionList(const QStringList& functionList);

protected:
    void closeEvent(QCloseEvent* event) override;

signals:
    void functionClicked(int index);

private:
    Ui::FunctionDialog* ui = nullptr;
};
