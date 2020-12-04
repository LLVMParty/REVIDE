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
    Ui::MainWindow* ui = nullptr;
    Webserver* mWebserver = nullptr;
    QList<QDialog*> mDialogs;
};
