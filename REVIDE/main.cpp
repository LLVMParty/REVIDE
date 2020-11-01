#include "MainWindow.h"

#include <QApplication>

#include <llvm/IR/Module.h>

int main(int argc, char *argv[])
{
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);
    QApplication a(argc, argv);
	a.setStyleSheet("CustomStyles { qproperty-testColor: rgb(100, 200, 100); }");
    MainWindow w;
    w.show();
    return a.exec();
}
