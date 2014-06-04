#-------------------------------------------------
#
# Project created by QtCreator 2012-07-13T23:48:42
#
#-------------------------------------------------

QT += core gui sql xml

TARGET = schedule_editor
TEMPLATE = app

SOURCES += \
main.cpp\
App.cpp \
logger.cpp \
ScheduleEditor.cpp \
SettingsEditor.cpp \
ReferenceEditor.cpp \
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
StudGroupDialog.cpp \
ActivityDialog.cpp \
ScheduleDatesDialog.cpp \
DocxParser.cpp \
Utils.cpp \
ScheduleModel.cpp \
XmlConfigModel.cpp \
DictionaryModel.cpp \
tetrismodel.cpp

HEADERS += \
App.h \
logger.h \
SettingsEditor.h \
ScheduleEditor.h \
ReferenceEditor.h \
domain/User.h \
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
StudGroupDialog.h \
ActivityDialog.h \
ScheduleDatesDialog.h \
DocxParser.h \
Utils.h \
ScheduleModel.h \
XmlConfigModel.h \
DictionaryModel.h \
tetrismodel.h

FORMS += \
ScheduleEditor.ui \
ReferenceEditor.ui \
mainwindow.ui \
StudGroupDialog.ui \
ActivityDialog.ui \
ScheduleDatesDialog.ui

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

RESOURCES += ImageResourses.qrc

OTHER_FILES += \
images/schedule.png \
images/information.png \
images/exit.png \
images/dictionary.png \
images/success_confirm.png \
images/remove.png \
images/confirm.png \
images/add.png \
images/menu.png \
images/refresh.png \
images/security_settings.png \
images/db_upload.png \
images/db_unload.png \
images/connect_settings.png
