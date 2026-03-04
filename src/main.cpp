#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QFontDatabase>
#include "View/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Sudoku Assistant");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("UniCaen");

    QFontDatabase::addApplicationFont(":/fonts/Nunito/static/Nunito-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Nunito/static/Nunito-Light.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Nunito/static/Nunito-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Nunito/static/Nunito-Medium.ttf");

    QFont appFont("Nunito", 13);
    app.setFont(appFont);

    // Load translation matching the system locale
    static QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        if (translator.load(":/i18n/sudoku_" + QLocale(locale).name() + ".qm")) {
            app.installTranslator(&translator);
            break;
        }
    }

    MainWindow window;
    window.show();

    return app.exec();
}
