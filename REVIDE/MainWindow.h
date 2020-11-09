#pragma once

#include <QMainWindow>
#include <QList>
#include <QDialog>
#include <QDir>
#include "Webserver.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void llvmResource(const char* resourcePath);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void helloSlot(QString ip);
    void llvmSlot(QString type, QString title, QByteArray data);

    void on_action_MinimalExample_triggered();

private:
    void initializeThemes();
    void initializeExamples(const QDir& dir, QMenu* menu);

private:
    Ui::MainWindow* ui = nullptr;
    Webserver* mWebserver = nullptr;
    QList<QDialog*> mDialogs;
};
