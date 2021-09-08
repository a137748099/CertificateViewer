#include "mainwidget.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    // look up e.g. :/translations/myapp_de.qm
    if (translator.load(":/linguist/linguist_cn.qm"))
        a.installTranslator(&translator);

    MainWidget w;
    w.show();
    return a.exec();
}
