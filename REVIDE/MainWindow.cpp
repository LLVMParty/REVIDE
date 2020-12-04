#include "MainWindow.h"
#include "BitcodeDialog.h"
#include "VTILDialog.h"

#include <QFileDialog>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <QMenuBar>

MainWindow::MainWindow(int port, QWidget* parent)
    : KDDockWidgets::MainWindow("REVIDE", KDDockWidgets::MainWindowOption_HasCentralFrame, parent)
{
    menu_File = menuBar()->addMenu(tr("&File"));
    menu_Examples = menuBar()->addMenu(tr("E&xamples"));
    menu_Theme = menuBar()->addMenu(tr("&Theme"));
    action_Open = menu_File->addAction(tr("&Open"));
    plainTextLog = new QPlainTextEdit(this);

    auto logDockWidget = new KDDockWidgets::DockWidget("ServerLog");
    logDockWidget->setWindowTitle(tr("Server log")); // TODO: this is not working
    logDockWidget->setWidget(plainTextLog);
    addDockWidgetAsTab(logDockWidget);

    initializeThemes();
    initializeExamples(QDir(":/examples"), menu_Examples);

    // Start the server
    mWebserver = new Webserver(port, this);
    connect(mWebserver, &Webserver::hello, this, &MainWindow::helloSlot);
    connect(mWebserver, &Webserver::llvm, this, &MainWindow::llvmSlot);
    mWebserver->start();

    // File -> Open
    connect(action_Open, &QAction::triggered, [this]() {
        // TODO: save this directory in settings (project directory)
        static QString dir = "projects";
        auto irFile = QFileDialog::getOpenFileName(this, "Caption", dir, "REVIDE (*.bc *.ll *.vtil);;All Files (*)");
        if (irFile.isEmpty())
            return;
        loadFile(QFileInfo(irFile));
    });
}

MainWindow::~MainWindow()
{
    mWebserver->close();
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
    else if (extension == "vtil")
    {
        vtilSlot(file.baseName(), contents);
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
    for (QDialog* dialog : mDialogs)
        dialog->close();
    QMainWindow::closeEvent(event);
}

void MainWindow::helloSlot(QString message)
{
    plainTextLog->appendPlainText(message);
}

void MainWindow::llvmSlot(QString type, QString title, QByteArray data)
{
    plainTextLog->appendPlainText(QString("llvm %1 (%2), %3 bytes").arg(type).arg(title).arg(data.length()));
    auto bitcodeDialog = new BitcodeDialog(nullptr);
    if (!title.isEmpty())
        bitcodeDialog->setWindowTitle(QString("[%1] %2 (%3)").arg(mDialogs.size() + 1).arg(bitcodeDialog->windowTitle()).arg(title));
    QString errorMessage;
    if (!bitcodeDialog->load(type, data, errorMessage))
    {
        plainTextLog->appendPlainText(QString("Failed to load LLVM module: %1").arg(errorMessage));
    }
    mDialogs.append(bitcodeDialog);
    bitcodeDialog->show();
    //bitcodeDialog->raise();
    //bitcodeDialog->activateWindow();
}

void MainWindow::vtilSlot(QString title, QByteArray data)
{
    plainTextLog->appendPlainText(QString("vtil routine (%1), %2 bytes").arg(title).arg(data.length()));
    auto dialog = new VTILDialog(nullptr);
    if (!title.isEmpty())
        dialog->setWindowTitle(QString("[%1] %2 (%3)").arg(mDialogs.size() + 1).arg(dialog->windowTitle()).arg(title));
    QString errorMessage;
    if (!dialog->load(data, errorMessage))
    {
        plainTextLog->appendPlainText(QString("Failed to load VTIL routine: %1").arg(errorMessage));
    }
    mDialogs.append(dialog);
    dialog->show();
}

void MainWindow::initializeThemes()
{
    for (const auto& theme : QDir(":/themes").entryInfoList())
        addThemeFile(theme);
    addThemeFile(QFileInfo("REVIDE.css"));
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
void MainWindow::addThemeFile(const QFileInfo& theme)
{
    if (!theme.exists())
        return;

    auto action = menu_Theme->addAction(theme.baseName());
    action->setCheckable(true);

    if (theme.filePath() == QSettings().value("Theme").toString())
        action->setChecked(true);

    connect(action, &QAction::triggered, [this, theme, action]() {
        QFile f(theme.filePath());
        if (!f.open(QFile::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Failed to read theme file %1").arg(f.fileName()));
            return;
        }

        qApp->setStyleSheet(f.readAll());
        QSettings().setValue("Theme", f.fileName());

        for (QAction* menuAction : menu_Theme->actions())
            menuAction->setChecked(false);
        action->setChecked(true);
    });
}
