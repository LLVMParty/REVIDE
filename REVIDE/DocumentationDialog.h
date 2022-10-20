#pragma once

#include <QDialog>

#include "AbstractFunctionList.h"

namespace Ui
{
class DocumentationDialog;
}

class DocumentationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocumentationDialog(QWidget* parent = nullptr);
    ~DocumentationDialog();
    void setHtml(const QString& html);

private:
    Ui::DocumentationDialog* ui = nullptr;
};

