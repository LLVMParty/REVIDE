#pragma once

#include <QMainWindow>
#include <QList>
#include <QDialog>
#include <QDir>
#include <QPlainTextEdit>
#include <QAction>
#include <QMenu>

#include "Webserver.h"
#include <kddockwidgets/DockWidget.h>
#include <kddockwidgets/MainWindow.h>

class MainWindow : public KDDockWidgets::MainWindow
{
    Q_OBJECT

public:
    MainWindow(int port, QWidget* parent = nullptr);
    ~MainWindow();

    void loadFile(const QFileInfo& file);
    void noServer();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void helloSlot(QString message);
    void llvmSlot(QString type, QString title, QByteArray data);
    void vtilSlot(QString title, QByteArray data);

private:
    void addThemeFile(const QFileInfo& theme);
    void initializeThemes();
    void initializeExamples(const QDir& dir, QMenu* menu);

private:
    Webserver* mWebserver = nullptr;
    QList<QDialog*> mDialogs;

    QMenu* menu_Examples = nullptr;
    QMenu* menu_File = nullptr;
    QAction* action_Open = nullptr;
    QMenu* menu_Theme = nullptr;
    QPlainTextEdit* plainTextLog = nullptr;
};
