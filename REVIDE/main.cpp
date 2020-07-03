#include "MainWindow.h"

#include <QApplication>

#include <llvm/IR/Module.h>

int main(int argc, char *argv[])
{
    llvm::LLVMContext context;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
