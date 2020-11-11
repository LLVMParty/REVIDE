#include "MainWindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QCommandLineParser>

#include <llvm/IR/Module.h>

int main(int argc, char* argv[])
{
    // Disable buffering because stdout/stderr are only used for debugging
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);

    // Create application
    QApplication app(argc, argv);
    QApplication::setOrganizationName("LLVMParty");
    QApplication::setApplicationName("REVIDE");

    // Parse command line
    // https://doc.qt.io/qt-5/qcommandlineparser.html#details
    QCommandLineParser parser;
    parser.setApplicationDescription("REVerse engineering IDE");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("files", QCoreApplication::translate("main", "File to open, optionally"), "[files...]");
    QCommandLineOption noServer("noserver", QCoreApplication::translate("main", "Don't start the server and show the main window"));
    parser.addOption(noServer);
    parser.process(app);

    // Set font to alias per default
    // https://stackoverflow.com/a/29588359/1806760
    {
        QFont font = QApplication::font();
        font.setStyleStrategy(QFont::PreferAntialias);
        QApplication::setFont(font);
    }

    // Handle theme settings
    {
        QSettings settings;
        const QString themeKey("Theme");
        auto themeFile = settings.value(themeKey).toString();
        const auto defaultTheme = ":/themes/Light.css";
        if(themeFile.isEmpty())
        {
            themeFile = defaultTheme;
            settings.setValue(themeKey, themeFile);
            settings.sync();
        }
        QFile f(themeFile);
        if(!f.open(QFile::ReadOnly))
        {
            f.setFileName(defaultTheme);
            f.open(QFile::ReadOnly);
            settings.setValue(themeKey, QVariant());
            settings.sync();
        }
        app.setStyleSheet(f.readAll());
        // TODO: if stylesheet parsing fails Qt continues with a warning
    }

    // Create main window
    MainWindow w;

    // Load the files specified on the command line
    // TODO: use http requests when another instance is already open
    for(const auto& file : parser.positionalArguments())
        w.loadFile(file);

    // Handle the --noserver command line
    if(parser.isSet(noServer))
        w.noServer();
    else
        w.show();

    // Run application
    return QApplication::exec();
}
