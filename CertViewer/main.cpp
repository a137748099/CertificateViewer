#include "mainwidget.h"

#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    if (translator.load(":/linguist/linguist_cn.qm"))
        a.installTranslator(&translator);

    MainWidget w;
    w.show();
    return a.exec();
}
