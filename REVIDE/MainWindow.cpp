#include "MainWindow.h"
#include "BitcodeDialog.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mWebserver = new Webserver(this);
    connect(mWebserver, &Webserver::hello, this, &MainWindow::helloSlot);
    connect(mWebserver, &Webserver::llvm, this, &MainWindow::llvmSlot);
    mWebserver->start();
}

MainWindow::~MainWindow()
{
    mWebserver->close();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    for(auto dialog : mDialogs)
        dialog->close();
    QMainWindow::closeEvent(event);
}

void MainWindow::helloSlot(QString ip)
{
    ui->plainTextLog->appendPlainText(QString("Hello from %1\n").arg(ip));
}

void MainWindow::llvmSlot(QString type, QString title, QByteArray data)
{
    ui->plainTextLog->appendPlainText(QString("llvm %1 (%2), %3 bytes").arg(type).arg(title).arg(data.length()));
    auto bitcodeDialog = new BitcodeDialog(nullptr);
    if(!title.isEmpty())
    bitcodeDialog->setWindowTitle(QString("[%1] %2 (%3)").arg(mDialogs.size() + 1).arg(bitcodeDialog->windowTitle()).arg(title));
    if(!bitcodeDialog->load(type, data))
    {
        ui->plainTextLog->appendPlainText("Failed to load data :(\n");
    }
    bitcodeDialog->show();
    mDialogs.append(bitcodeDialog);
}

