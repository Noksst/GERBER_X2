#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QGLWidget>
#include <QLocale>
#include <QOperatingSystemVersion>
#include <QSettings>
#include <QSplashScreen>
#include <QStandardPaths>
#include <QTranslator>

#include "mainwindow.h"
#include "version.h"

void initIcon();
void translation(QApplication* app);

int main(int argc, char* argv[])
{
    Q_INIT_RESOURCE(resources);
    QApplication app(argc, argv);

    app.setApplicationName("G2G");
    app.setOrganizationName(VER_COMPANYNAME_STR);
    app.setApplicationVersion(VER_PRODUCTVERSION_STR);

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, app.applicationDirPath());

    QGLFormat glf = QGLFormat::defaultFormat();
    glf.setSampleBuffers(true);
    glf.setSamples(16);
    QGLFormat::setDefaultFormat(glf);

    initIcon();
    translation(&app);

    QSplashScreen* splash = nullptr;
    splash = new QSplashScreen(QPixmap(QLatin1String(":/256.png")));
    splash->setAttribute(Qt::WA_DeleteOnClose);
    splash->show();

    MainWindow mainWin;
    mainWin.setIconSize({ 24, 24 });
    mainWin.show();
    splash->finish(&mainWin);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file(s) to open.");
    parser.process(app);

    return app.exec();
}

void initIcon()
{
    QIcon::setThemeSearchPaths({
        "../icons/",
        "../icons/breeze/",
        "icons/",
        "icons/breeze/",
    });
    QIcon::setThemeName("Breeze");
}

void translation(QApplication* app)
{
    const QString loc(QLocale().name().left(2));
    qDebug() << "locale:" << loc;
    QString trFolder;

    if (qApp->applicationDirPath().contains("GERBER_X2/bin"))
        trFolder = "../G2G/translations/"; // for debug
    else
        trFolder = (qApp->applicationDirPath() + "/translations/");

    auto translator = [app](const QString& path) {
        if (QFile::exists(path)) {
            QTranslator* translator = new QTranslator();
            if (translator->load(path))
                app->installTranslator(translator);
            else
                delete translator;
        }
    };

    translator(trFolder + qApp->applicationDisplayName().toLower() + "_" + loc + ".qm");
    translator(trFolder + "qtbase_" + loc + ".qm");
    translator(trFolder + "qt_" + loc + ".qm");
}
