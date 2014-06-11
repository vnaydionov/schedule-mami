#ifndef SCHEDULEMANAGER_H
#define SCHEDULEMANAGER_H

#include <QVector>
#include "documentparser.h"
#include "mainwindow.h"
#include <QStringList>
#include <orm/MetaData.h>
#include "domain/User.h"
#include "sharedobj.h"
#include <QMap>
#include "domain/Schedule.h"
#include "domain/Place.h"
#include <QHash>
#include "domain/Activity.h"

class ScheduleManager
{
    typedef void(ScheduleManager::*Handler)(int, int);
    Yb::Session *session_;
    Domain::User *user_;
    QMap<QString, Handler> handlers_;

    QStringList categories_;
    QVector<Row> table_;
    bool specialCaseProcessed_;

    Domain::ScheduleHolder *scheduleHolder_;

    void dowHandler(int row, int column);
    void subjectHandler(int row, int column);
    void placeHandler(int row, int column);
    void timeHandler(int row, int column);
    void dateHandler(int row, int column);
    void courseHandler(int row, int column);
public:   


    ScheduleManager(Domain::User *user);
    void convertToScheduleObj(bool save = false);

    ~ScheduleManager()
    {
        delete session_;
    }
};

#endif // SCHEDULEMANAGER_H
