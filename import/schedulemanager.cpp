#include "schedulemanager.h"

using namespace Domain;
using namespace Yb;

ScheduleManager::ScheduleManager(Domain::User *user):
    specialCaseProcessed_(false)
{
    user_ = user;
    session_ = new Yb::Session(Yb::theSchema()
                               , SharedObjSingleton::instance().getEngine());

    handlers_.insert(_R("День"), &ScheduleManager::dowHandler);
    handlers_.insert(_R("Место"), &ScheduleManager::placeHandler);
    handlers_.insert(_R("Курс"), &ScheduleManager::courseHandler);
    handlers_.insert(_R("Дисциплина"), &ScheduleManager::subjectHandler);
    handlers_.insert(_R("Дата"), &ScheduleManager::dateHandler);
    handlers_.insert(_R("Время"), &ScheduleManager::timeHandler);

    table_ = SharedObjSingleton::instance().getRows();
    categories_ = SharedObjSingleton::instance().getCategories();
}

void
ScheduleManager::convertToScheduleObj(bool save)
{
    for(int i = 0; i < table_.size(); ++i)
    {
        specialCaseProcessed_ = false;
        scheduleHolder_ = new ScheduleHolder(*session_);
        for(int j = 0; j < table_.at(0).size(); ++j)
        {
            Handler handler = handlers_.value(categories_.at(j));
            (this->*handler)(i, j);
        }
        if(save)
            session_->commit();
        else
            session_->rollback();

        //qDebug() << "Repeat: " << (*scheduleHolder_)->repeat_type.value();
        //qDebug() << "Date start: " << (*scheduleHolder_)->start_dt.value();
        //qDebug() << "Date end: " << (*scheduleHolder_)->end_dt.value();
        //qDebug() << "Place: " << (*scheduleHolder_)->place->name.value();
        //qDebug() << "Time start: " << (*scheduleHolder_)->start_time.value();
        //qDebug() << "Time end: " << (*scheduleHolder_)->end_time.value();
    }
}


void
ScheduleManager::dowHandler(int row, int column)
{
    Cell cell = table_.at(row).at(column);
    (*scheduleHolder_)->dow = cell.concatContent();
}

void
ScheduleManager::dateHandler(int row, int column)
{
    Cell cell = table_.at(row).at(column);
    Cell subjCell = table_.at(row)
            .at(categories_.indexOf(_R("Дисциплина")));
    if(subjCell.size() > 1)
        return;
    QStringList dates = cell.concatContent("").split("-");
    (*scheduleHolder_)->start_dt = QDateTime::fromString(dates[0], "dd.MM");
    (*scheduleHolder_)->end_dt = QDateTime::fromString(dates[1], "dd.MM");
}

void
ScheduleManager::subjectHandler(int row, int column)
{
    Cell cell = table_.at(row).at(column);
    Cell timeCell = table_.at(row).at(categories_.indexOf(_R("Время")));
    Cell dateCell = table_.at(row).at(categories_.indexOf(_R("Дата")));
    if (cell.size() > 1 && !specialCaseProcessed_)
    {
        QString subject_name = cell.blocks_.at(0);
        for(int i = 1; i < cell.size(); ++i)
        {
            ScheduleHolder tmpSchedule(*session_);
            SubjectHolder subject(*session_);
            subject->name = subject_name;
            tmpSchedule->subject = subject;
            tmpSchedule->repeat_type = "once";
            Cell tcell = table_.at(row).at(categories_.indexOf(_R("День")));
            tmpSchedule->dow = tcell.concatContent("");
            PlaceHolder place(*session_);
            tcell = table_.at(row).at(categories_.indexOf(_R("Место")));
            place->name = tcell.concatContent("");
            place->short_name = tcell.concatContent("");
            tmpSchedule->place = place;
            QString date = dateCell.blocks_.at(i);
            tmpSchedule->start_dt = QDateTime::fromString(date, "dd.MM");
            tmpSchedule->end_dt = QDateTime::fromString(date, "dd.MM");
            QString time = timeCell.concatContent("");
            tmpSchedule->start_time = time.toInt();
            tmpSchedule->end_time = time.toInt() + 5;
        }
        specialCaseProcessed_ = true;
    }
    else
    {
        QString text = cell.concatContent("");
        if(text.contains(_R("лаб.")))
        {
            ActivityHolder activity(*session_);
            activity->name = _R("Лабораторые");
            activity->short_name = _R("Лабораторые");
            //(*scheduleHolder_)->activities.insert(*activity);
        }
        if(text.contains(_R("практ.")))
        {
            ActivityHolder activity(*session_);
            activity->name = _R("Практика");
            activity->short_name = _R("Практика");
            //(*scheduleHolder_)->activities.insert(*activity);
        }
        if(text.contains(_R("лекции")))
        {
            ActivityHolder activity(*session_);
            activity->name = _R("Лекция");
            activity->short_name = _R("Лекция");
            //(*scheduleHolder_)->activities.insert(*activity);
        }
        if(text.contains(_R("консультации")))
        {
            ActivityHolder activity(*session_);
            activity->name = _R("Консультация");
            activity->short_name = _R("Консультация");
           // (*scheduleHolder_)->activities.insert(*activity);
        }
        SubjectHolder subject(*session_);
        subject->name = text;
        (*scheduleHolder_)->subject = subject;
        (*scheduleHolder_)->repeat_type = "each";
    }
    //qDebug() << "Subject name: " << (*scheduleHolder_)->subject->name.value();
}

void
ScheduleManager::courseHandler(int row, int column)
{
    /*
     * Я хз что тут делать, просто курса в базе нет.
     */
}

void
ScheduleManager::timeHandler(int row, int column)
{
    Cell cell = table_.at(row).at(column);
    Cell subjCell = table_.at(row)
            .at(categories_.indexOf(_R("Дисциплина")));
    if(subjCell.size() > 1)
        return;
    if(cell.size() == 1)
    {
        QString time1 = cell.blocks_.at(0);
        QStringList times = time1.split("-");
        (*scheduleHolder_)->start_time = times[0].remove(".").toInt();
        (*scheduleHolder_)->end_time = times[1].remove(".").toInt();
    }
    else
    {
        QString time1 = cell.blocks_.first();
        QString time2 = cell.blocks_.last();
        QString time_end = "";
        if (time2.contains("-"))
            time_end = time2.split("-").at(1);
        else
            time_end = time1.split("-").at(1);
        QString time_start = time1.split("-").at(0);
        (*scheduleHolder_)->start_time = time_start.remove(".").toInt();
        (*scheduleHolder_)->end_time = time_end.remove(".").toInt();
    }
}

void
ScheduleManager::placeHandler(int row, int column)
{
    Cell cell = table_.at(row).at(column);
    QString name = cell.concatContent("");

    DomainResultSet<Place> rs = query<Place>(*session_)
                        .filter_by(Place::c.name == name || Place::c.short_name == name)
                        .all();
    DomainResultSet<Place>::iterator it = rs.begin();
    if (it == rs.end()) {
        bool ok = true;
        QString msg = _R("В базе не найдена аудитория: ") + name
                     + _R("\nВыберете вариант");
        QStringList places;
        places << _R("Добавить: ") + name;
        QHash<QString, qlonglong> ids;
        DomainResultSet<Place> rs = query<Place>(*session_).all();
        DomainResultSet<Place>::iterator it = rs.begin();
        for (; it != rs.end(); ++it) {
            places << it->name.value();
            ids.insert(it->name.value(), it->id.value());
        }
        QString place = QInputDialog::getItem(0, _R("Выбор аудитории"),
                                                msg,
                                                places,
                                                ok);
        if (ok)
        {
            if (place.contains(_R("Добавить")))
            {
                PlaceHolder placeHolder(*session_);
                placeHolder->name = name;
                placeHolder->short_name = name;
                (*scheduleHolder_)->place = placeHolder;
            }
            else
            {
                (*scheduleHolder_)->place = PlaceHolder(*session_, ids.value(place));
            }
        }
    } else
        (*scheduleHolder_)->place = PlaceHolder(*session_, it->id);
}
