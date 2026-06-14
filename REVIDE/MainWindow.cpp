#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "BitcodeDialog.h"
#include "ui_MainWindow.h"
#include "QtHelpers.h"
#include "DarkTheme.h"

#include <QFileDialog>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>

MainWindow::MainWindow(int port, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mDockManager = new ads::CDockManager(this);
    mDockManager->setStyleSheet(DarkTheme::dockManagerStyleSheet());

    auto dockWidget = new ads::CDockWidget(mDockManager, "Log");
    dockWidget->setWidget(ui->plainTextLog);
    dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, false);
    mDockManager->addDockWidgetTab(ads::TopDockWidgetArea, dockWidget);

    qtRestoreGeometry(this);
    qtRestoreState(this);

    initializeExamples(QDir(":/examples"), ui->menu_Examples);

    // Start the server
    mWebserver = new Webserver(port, this);
    connect(mWebserver, &Webserver::hello, this, &MainWindow::helloSlot);
    connect(mWebserver, &Webserver::llvm, this, &MainWindow::llvmSlot);
    mWebserver->start();

    // File -> Open
    connect(ui->action_Open, &QAction::triggered, [this]() {
        // TODO: save this directory in settings (project directory)
        static QString dir = "projects";
        auto irFile = QFileDialog::getOpenFileName(this, "Caption", dir, "REVIDE (*.bc *.ll);;All Files (*)");
        if (irFile.isEmpty())
            return;
        loadFile(QFileInfo(irFile));
    });
    connect(ui->action_About, &QAction::triggered, [this]() {
        QMessageBox::information(this, tr("About"), tr("REVIDE (development version)\n\nCreated by: Duncan Ogilvie\n\nThis is a work in progress!"));
    });
}

MainWindow::~MainWindow()
{
    mWebserver->close();
    delete ui;
}

void MainWindow::loadFile(const QFileInfo& file)
{
    // Read the file contents
    QByteArray contents;
    {
        QFile f(file.absoluteFilePath());
        if (!f.open(QFile::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Failed to open file: \"%1\"").arg(f.fileName()));
            return;
        }
        contents = f.readAll();
    }

    // Dispatch to the right handler
    auto extension = file.suffix();
    if (extension == "bc" || extension == "ll")
    {
        llvmSlot("module", file.baseName(), contents);
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("%1 is not a recognized file extension.").arg(extension));
    }
}

void MainWindow::noServer()
{
    mDialogs.clear();
    close();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    qtSaveGeometry(this);
    qtSaveState(this);
    for (auto dialog : mDialogs)
        dialog->close();
    QMainWindow::closeEvent(event);
}

void MainWindow::helloSlot(QString message)
{
    ui->plainTextLog->appendPlainText(message);
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

    auto dockWidget = new ads::CDockWidget(mDockManager, bitcodeDialog->windowTitle());
    dockWidget->setWidget(bitcodeDialog);
    mDockManager->addDockWidgetTab(ads::TopDockWidgetArea, dockWidget);
    //bitcodeDialog->show();
    //bitcodeDialog->raise();
    //bitcodeDialog->activateWindow();
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
                loadFile(entry);
            });
        }
    }
}
