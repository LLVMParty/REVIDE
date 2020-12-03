#include "FunctionDialog.h"
#include "ui_FunctionDialog.h"
#include "FunctionListModel.h"

#include <QMessageBox>

FunctionDialog::FunctionDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::FunctionDialog)
{
    setWindowFlags(Qt::Tool);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->setupUi(this);

    connect(ui->functionList, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
        auto index = ui->functionList->currentIndex().row();
        emit functionClicked(index);
    });
    // TODO: save position and state based on the parent object id (BitcodeDialog)
}

FunctionDialog::~FunctionDialog()
{
    delete ui;
}

#include <QStringListModel>

void FunctionDialog::setFunctionList(const QStringList& functionList)
{
    ui->functionList->clear();
    ui->functionList->addItems(functionList);
}
