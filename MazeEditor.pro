#-------------------------------------------------
#
# Project created by QtCreator 2018-11-20T14:01:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MazeEditor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mazeeditor.cpp \
    mainctrl.cpp \
    msgcenter.cpp \
    setbasicinfo.cpp \
    formmgr.cpp \
    basejsonhelper.cpp \
    globalfunc.cpp \
    tipsdialog.cpp\
    excelres.cpp \
    excelbase.cpp \
    dlgopenmaze.cpp \
    cusrect.cpp \
    cusarrow.cpp \
    cusscene.cpp \
    dlgsetstage.cpp \
    cusview.cpp \
    dlgsetoption.cpp \
    dlgconfirm.cpp \
    publishhelper.cpp

HEADERS += \
        mazeeditor.h \
    mainctrl.h \
    msgcenter.h \
    setbasicinfo.h \
    formmgr.h \
    basejsonhelper.h \
    globalfunc.h \
    tipsdialog.h\
    excelres.h \
    excelbase.h \
    dlgopenmaze.h \
    cusrect.h \
    cusarrow.h \
    cusscene.h \
    dlgsetstage.h \
    cusview.h \
    dlgsetoption.h \
    dlgconfirm.h \
    publishhelper.h

FORMS += \
        mazeeditor.ui \
    setbasicinfo.ui \
    tipsdialog.ui \
    dlgopenmaze.ui \
    dlgsetstage.ui \
    dlgsetoption.ui \
    dlgconfirm.ui

QT += axcontainer

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
