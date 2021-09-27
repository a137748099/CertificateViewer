#include "mainwidget.h"
#include "tool/logger.h"

#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef QT_NO_DEBUG
    auto logPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/CertViewer/Log/";
    Logger::installLog(logPath);
#endif

    QTranslator translator;
    if (translator.load(":/linguist/linguist_cn.qm"))
        a.installTranslator(&translator);

    qDebug() << __FUNCTION__ << "song" << argc;
    QString certPath;
    for(int i = 0; i < argc; ++i) {
        qDebug() << __FUNCTION__ << "song" << argv[i];
        QString filePath = QString::fromLocal8Bit(argv[i]);
        QFileInfo fileInfo(filePath);
        qDebug() << __FUNCTION__ << "song" << fileInfo.isFile() << fileInfo.suffix();
        if(fileInfo.isFile() && (fileInfo.suffix() == "cer" || fileInfo.suffix() == "crt" || fileInfo.suffix() == "pem")) {
            certPath = filePath;
            qDebug() << __FUNCTION__ << "song" << "certPath: " << certPath ;
        }
    }

    MainWidget w;
    if(!certPath.isEmpty()) {
        w.openFile(certPath);
    }
    w.show();
    return a.exec();
}
