#include "MainWindow.h"
#include "BitcodeDialog.h"
#include "ui_MainWindow.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QSettings>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initializeThemes();
    initializeExamples(QDir(":/examples"), ui->menu_Examples);

    // Start the server
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

void MainWindow::llvmResource(const char* resourcePath)
{
    auto type = "module"; // TODO
    QFile f(resourcePath);
    if (!f.open(QFile::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open resource file"));
        return;
    }
    auto title = QFileInfo(f).baseName();
    QByteArray data = f.readAll();
    llvmSlot(type, title, data);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    for (QDialog* dialog : mDialogs)
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
    if (!title.isEmpty())
        bitcodeDialog->setWindowTitle(QString("[%1] %2 (%3)").arg(mDialogs.size() + 1).arg(bitcodeDialog->windowTitle()).arg(title));
    QString errorMessage;
    if (!bitcodeDialog->load(type, data, errorMessage))
    {
        ui->plainTextLog->appendPlainText(QString("Failed to load LLVM module: %1").arg(errorMessage));
    }
    mDialogs.append(bitcodeDialog);
    bitcodeDialog->show();
    //bitcodeDialog->raise();
    //bitcodeDialog->activateWindow();
}

void MainWindow::on_action_MinimalExample_triggered()
{
    llvmResource(":/examples/LLVM/WhitePeacock/MinimalExample.bc");
}

void MainWindow::initializeThemes()
{
    auto selectedTheme = QSettings().value("Theme").toString();
    for (const auto& theme : QDir(":/themes").entryInfoList())
    {
        auto name = theme.baseName();
        auto action = ui->menu_Theme->addAction(name);
        action->setCheckable(true);

        if (name == selectedTheme)
            action->setChecked(true);

        connect(action, &QAction::triggered, [this, theme, action]() {
            QFile f(theme.filePath());
            f.open(QFile::ReadOnly);
            qApp->setStyleSheet(f.readAll());

            QSettings().setValue("Theme", theme.baseName());
            for (QAction* menuAction : ui->menu_Theme->actions())
                menuAction->setChecked(false);
            action->setChecked(true);
        });
    }
}

void MainWindow::initializeExamples(const QDir& dir, QMenu* menu)
{
    for (const auto& entry : dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst))
    {
        if (entry.isDir())
        {
            initializeExamples(QDir(entry.absoluteFilePath()), menu->addMenu(entry.baseName()));
        }
        else
        {
            auto action = menu->addAction(entry.baseName());
            connect(action, &QAction::triggered, [this, entry]() {
                // Read the file contents
                QByteArray contents;
                {
                    QFile f(entry.absoluteFilePath());
                    if (!f.open(QFile::ReadOnly))
                    {
                        QMessageBox::critical(this, tr("Error"), tr("Failed to open resource file"));
                        return;
                    }
                    contents = f.readAll();
                }

                // Dispatch to the right handler
                auto extension = entry.completeSuffix();
                if (extension == "bc" || extension == "ll")
                {
                    llvmSlot("module", entry.baseName(), contents);
                }
                else
                {
                    QMessageBox::critical(this, tr("Error"), tr("%1 is not a recognized example extension.").arg(extension));
                }
            });
        }
    }
}
