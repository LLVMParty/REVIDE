#include "MainWindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QSettings>

#include <llvm/IR/Module.h>

int main(int argc, char* argv[])
{
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);
    QApplication a(argc, argv);
    a.setOrganizationName("LLVMParty");
    a.setApplicationName("REVIDE");
    {
        QSettings settings;
        const QString themeKey("Theme");
        auto theme = settings.value(themeKey).toString();
        if(theme.isEmpty())
        {
            theme = "Light";
            settings.setValue(themeKey, theme);
            settings.sync();
        }
        QFile f(QString(":/themes/%1.css").arg(theme));
        if(!f.open(QFile::ReadOnly))
        {
            f.setFileName(":/themes/Light.css");
            if(!f.open(QFile::ReadOnly))
            {
                return EXIT_FAILURE;
            }
            settings.setValue(themeKey, QVariant());
            settings.sync();
        }
        a.setStyleSheet(f.readAll());
        // TODO: if stylesheet parsing fails Qt continues with a warning
    }
    MainWindow w;
    w.show();
    return a.exec();
}
