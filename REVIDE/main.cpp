#include "MainWindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QCommandLineParser>

#include "core/Cutter.h"
#include "common/Configuration.h"
#include <llvm/IR/Module.h>

int main(int argc, char* argv[])
{
    // Disable buffering because stdout/stderr are only used for debugging
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);

    // Hopefully enable high DPI support
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

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
    QCommandLineOption paramNoServer("noserver", QCoreApplication::translate("main", "Don't start the server and show the main window"));
    parser.addOption(paramNoServer);
    int port = 13337;
    QCommandLineOption paramPort("port", QCoreApplication::translate("main", "Port to listen on (defaults to %1)").arg(port), "port");
    parser.addOption(paramPort);
    parser.addPositionalArgument("files", QCoreApplication::translate("main", "File(s) to open, optionally"), "[files...]");
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
        if (themeFile.isEmpty())
        {
            themeFile = defaultTheme;
            settings.setValue(themeKey, themeFile);
            settings.sync();
        }
        QFile f(themeFile);
        if (!f.open(QFile::ReadOnly))
        {
            f.setFileName(defaultTheme);
            f.open(QFile::ReadOnly);
            settings.setValue(themeKey, QVariant());
            settings.sync();
        }
        app.setStyleSheet(f.readAll());
        // TODO: if stylesheet parsing fails Qt continues with a warning
    }

    // Create Cutter instance
    CutterCore core(nullptr);
    Config()->loadInitial();

    // Handle a custom port
    if (parser.isSet(paramPort))
        port = parser.value(paramPort).toInt();
    else
        port = QSettings().value("Port", port).toInt();

    // Create main window
    MainWindow w(port);

    // Load the files specified on the command line
    // TODO: use http requests when another instance is already open
    for (const auto& file : parser.positionalArguments())
        w.loadFile(QFileInfo(file));

    // Handle the --noserver command line
    if (parser.isSet(paramNoServer))
        w.noServer();
    else
        w.show();

    // Run application
    return QApplication::exec();
}

#ifdef WIN32
#include <shellapi.h>
int WINAPI CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    // https://utf8everywhere.org/
    int argc = 0;
    auto argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    std::vector<QByteArray> argv_utf8;
    for (int i = 0; i < argc; i++)
        argv_utf8.push_back(QString::fromUtf16((const char16_t*)argv[i]).toUtf8());
    LocalFree(argv);
    std::vector<char*> argv_main;
    for (auto& arg : argv_utf8)
        argv_main.push_back(arg.data());
    return main(argc, argv_main.data());
}
#endif // WIN32