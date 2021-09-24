#include "mainwidget.h"

#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    // look up e.g. :/translations/myapp_de.qm
    if (translator.load(":/linguist/linguist_cn.qm"))
        a.installTranslator(&translator);

    enum string{
        x1,
        x2,
        x3=10,
        x4,
        x5,
    } x;

    qDebug() << __FUNCTION__ << "song" <<x;

    unsigned char *p1;
    unsigned long *p2;
    p1=(unsigned char *)0x801000;
    p2=(unsigned long *)0x810000;
    qDebug() << __FUNCTION__ << "song" << p1 + 5 << p2 + 5;

    MainWidget w;
    w.show();
    return a.exec();
}
