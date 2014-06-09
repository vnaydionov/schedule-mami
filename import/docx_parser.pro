#-------------------------------------------------
#
# Project created by QtCreator 2013-04-04T14:52:43
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = docx_parser
TEMPLATE = app


SOURCES += main.cpp\
    domain/User.cpp \
    domain/Subject.cpp \
    domain/StudGroup.cpp \
    domain/ScheduleGroup.cpp \
    domain/ScheduleDate.cpp \
    domain/ScheduleActivity.cpp \
    domain/Schedule.cpp \
    domain/Place.cpp \
    domain/LoginSession.cpp \
    domain/Activity.cpp \
    mainwindow.cpp \
    documentparser.cpp \
    tablemodel.cpp \
    classifier.cpp \
    delegate.cpp \
    category.cpp \
    schedulemanager.cpp \
    sharedobj.cpp

HEADERS  += domain/User.h \
    domain/Subject.h \
    domain/StudGroup.h \
    domain/ScheduleGroup.h \
    domain/ScheduleDate.h \
    domain/ScheduleActivity.h \
    domain/Schedule.h \
    domain/Place.h \
    domain/LoginSession.h \
    domain/Activity.h \
    mainwindow.h \
    documentparser.h \
    tablemodel.h \
    classifier.h \
    delegate.h \
    category.h \
    schedulemanager.h \
    sharedobj.h

FORMS    += mainwindow.ui

CONFIG += qt

LIBS += -L/home/vaclav/work/yb-orm.inst-qt/lib/ -lyborm -lybutil
INCLUDEPATH += /home/vaclav/work/yb-orm.inst-qt/include/yb
DEPENDPATH += /home/vaclav/work/yb-orm.inst-qt/include/yb

##LIBS += -L/home/vaclav/inst-qt/lib/ -lyborm -lybutil
##INCLUDEPATH += /home/vaclav/inst-qt/include/yb
##DEPENDPATH += /home/vaclav/inst-qt/include/yb

##LIBS += -L c:/yborm/lib/ -lyborm -lybutil
##INCLUDEPATH += c:/yborm/include/yb
##DEPENDPATH += c:/yborm/include/yb

sql-plugins += odbc

##INCLUDEPATH += /usr/include
##LIBS += -L/usr/lib/odbc/ -lodbc

DEFINES += YB_USE_QT


##INCLUDEPATH += C:\QtSDK\Desktop\Qt\4.8.1\src\gui\text
##INCLUDEPATH += C:\QtSDK\Desktop\Qt\4.8.1\src\3rdparty\zlib
INCLUDEPATH += /usr/include/src/gui/text
