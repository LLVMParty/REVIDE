#include "MainWindow.h"
#include "BitcodeDialog.h"
#include "ui_MainWindow.h"
#include <QCryptographicHash>

MainWindow::MainWindow(QWidget* parent)
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
    for (auto& [dialog, _] : mDialogs)
        dialog->close();
    QMainWindow::closeEvent(event);
}

void MainWindow::helloSlot(QString ip)
{
    ui->plainTextLog->appendPlainText(QString("Hello from %1\n").arg(ip));
}

void MainWindow::llvmSlot(QString type, QString title, QByteArray data)
{
    auto hash = QCryptographicHash::hash(data, QCryptographicHash::Sha1);
    auto found = std::find_if(mDialogs.begin(), mDialogs.end(), [&](const auto& e) {
        return e.second == hash;
    });
    QDialog* dialog = nullptr;
    if (found == mDialogs.end())
    {
        ui->plainTextLog->appendPlainText(QString("llvm %1 (%2), %3 bytes").arg(type).arg(title).arg(data.length()));
        auto bitcodeDialog = new BitcodeDialog(nullptr);
        if (!title.isEmpty())
            bitcodeDialog->setWindowTitle(QString("[%1] %2 (%3)").arg(mDialogs.size() + 1).arg(bitcodeDialog->windowTitle()).arg(title));
        QString errorMessage;
        if (!bitcodeDialog->load(type, data, errorMessage))
        {
            ui->plainTextLog->appendPlainText(QString("Failed to load LLVM module: %1").arg(errorMessage));
        }
        mDialogs.append({ bitcodeDialog, hash });
        dialog = bitcodeDialog;
    }
    else
    {
        dialog = found->first;
    }
    dialog->show();
    //dialog->raise();
    //dialog->activateWindow();
}
