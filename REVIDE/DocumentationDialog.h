#pragma once

#include <QDialog>

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

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::DocumentationDialog* ui = nullptr;
};
