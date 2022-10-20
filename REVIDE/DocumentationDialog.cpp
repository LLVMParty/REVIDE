#include "DocumentationDialog.h"
#include "ui_DocumentationDialog.h"
#include "FunctionListModel.h"

#include <QStringListModel>

#include <QMessageBox>

DocumentationDialog::DocumentationDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DocumentationDialog)
{
    setWindowFlags(Qt::Tool);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->setupUi(this);
}

DocumentationDialog::~DocumentationDialog()
{
    delete ui;
}

void DocumentationDialog::setHtml(const QString& html)
{
    ui->textBrowser->setHtml(html);
}
