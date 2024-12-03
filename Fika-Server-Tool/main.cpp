#include <QApplication>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load translations
    QTranslator translator;
    if (translator.load(":/translations/translation_zh_CN.qm")) {
        a.installTranslator(&translator);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
