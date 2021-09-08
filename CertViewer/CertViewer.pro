QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    detailwidget.cpp \
    generalwidget.cpp \
    main.cpp \
    mainwidget.cpp \
    tool/httpgetmanager.cpp \
    tree/treeitem.cpp \
    tree/treemodel.cpp

HEADERS += \
    detailwidget.h \
    generalwidget.h \
    mainwidget.h \
    tool/httpgetmanager.h \
    tree/treeitem.h \
    tree/treemodel.h \
    x509certificate.h

TRANSLATIONS += $$PWD/linguist/linguist_cn.ts

CONFIG(debug, debug|release) {#debug与release生成在不同的目录
    DESTDIR = $$PWD/../bin_debug
    LIBS += -L$$PWD/../bin_debug/ -lx509lib
}
CONFIG(release, debug|release) {
    DESTDIR = $$PWD/../bin_release
    LIBS += -L$$PWD/../bin_release/ -lx509lib
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    detailwidget.ui \
    generalwidget.ui

RESOURCES += \
    Resource.qrc
