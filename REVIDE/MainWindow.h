#pragma once

#include <QMainWindow>
#include <QList>
#include <QDialog>
#include "Webserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void helloSlot(QString ip);
    void llvmSlot(QString type, QString title, QByteArray data);

private:
    Ui::MainWindow *ui = nullptr;
    Webserver* mWebserver = nullptr;
    QList<QDialog*> mDialogs;
};
