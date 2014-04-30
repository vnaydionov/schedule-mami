#include "DocxParser.h"
#include "App.h"
#include <QProcess>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QInputDialog>

using namespace Yb;
using namespace Domain;

Yb::ElementTree::ElementPtr DocxParser::getXmlFromDocx(const QString &docx)
{
    Q_UNUSED(docx);
    QString work_path = QCoreApplication::applicationDirPath() + "/doc/";
    QProcess process;
    process.startDetached("sh " + work_path + "convert_docx_to_xml.sh");
    process.waitForFinished();
//  sleep(3);

    Yb::ElementTree::ElementPtr root;
    QFile file(work_path + "schedule.xml");
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        //Read table from schedule.xml.
        root = Yb::ElementTree::parse(file.readAll().data());
        file.close();
    } else
        qDebug() << "schedule.xml is wrong..";
    return root;
}

Yb::ElementTree::ElementsPtr DocxParser::getTableRowsFromXml(ElementTree::ElementPtr root)
{
    Yb::ElementTree::ElementPtr table = root->find_first("office:body");
    table = table->find_first("office:text");
    table = table->find_first("table:table");
    return table->find_all("table:table-row");
}

ScheduleHolder DocxParser::getRow(const Yb::ElementTree::ElementPtr &e, Yb::Session *session)
{
    ScheduleHolder s(*session);
    bool full_row = true;
    Yb::ElementTree::ElementsPtr cells = e->find_all("table:table-cell");
    Yb::ElementTree::Elements::iterator it_cell = cells->begin();
    int i=0;
    for (; it_cell != cells->end(); ++it_cell, ++i) {
        QString text = (*it_cell)->find_first("text:p")->get_text();
        if (text.isEmpty()) {
            full_row = false;
            break;
        }
        if (i == 0)
            s->dow = text;
        else if (i == 1) {
            text.remove('.');
            QStringList times = text.split('-');
            if (times.count() < 2) {
                full_row = false;
                break;
            }
            s->start_time = times.at(0).toInt();
            s->end_time = times.at(1).toInt();
        } else if (i == 2) {
            DomainResultSet<Subject> rs = query<Subject>(*session)
                    .filter_by(Subject::c.name == text)
                    .all();
            DomainResultSet<Subject>::iterator it = rs.begin();
            if (it == rs.end()) {
                bool ok = true;
                QString mess = QString::fromUtf8("В базе не найден следующий предмет: \"") + text
                             + QString::fromUtf8("\"\nВы можете выбрать один из существующих.");
                QStringList subjects;
                QHash<QString, qlonglong> ids;
                DomainResultSet<Subject> rs = query<Subject>(*session).all();
                DomainResultSet<Subject>::iterator it = rs.begin();
                for (; it != rs.end(); ++it) {
                    subjects.append(it->name);
                    ids.insert(it->name, it->id);
                }
                QString subj = QInputDialog::getItem(0, QString::fromUtf8("Парсер .docx"),
                                                        mess,
                                                        subjects,
                                                        ok);
                if (ok)
                    s->subject = SubjectHolder(*session, ids.value(subj));
            } else
                s->subject = SubjectHolder(*session, it->id);
        } else if (i == 3) {
            QStringList dates = text.split('-');
            if (dates.count() < 2) {
                full_row = false;
                break;
            }
            s->start_dt = QDateTime::currentDateTime();//QDateTime::fromString(times.at(0), "dd.MM.yyThh.mm.ss");
            s->end_dt = QDateTime::currentDateTime();//QDateTime::fromString(times.at(1), "dd.MM.yyThh.mm.ss");
        } else if (i == 4) {
            DomainResultSet<Place> rs = query<Place>(*session)
                    .filter_by(Place::c.name == text || Place::c.short_name == text)
                    .all();
            DomainResultSet<Place>::iterator it = rs.begin();
            if (it == rs.end()) {
                bool ok = true;
                QString mess = QString::fromUtf8("В базе не найдена следующая аудитория: \"") + text
                             + QString::fromUtf8("\"\nВы можете выбрать одну из существующих.");
                QStringList places;
                QHash<QString, qlonglong> ids;
                DomainResultSet<Place> rs = query<Place>(*session).all();
                DomainResultSet<Place>::iterator it = rs.begin();
                for (; it != rs.end(); ++it) {
                    places.append(it->name);
                    ids.insert(it->name, it->id);
                }
                QString place = QInputDialog::getItem(0, QString::fromUtf8("Парсер .docx"),
                                                        mess,
                                                        places,
                                                        ok);
                if (ok)
                    s->place = PlaceHolder(*session, ids.value(place));
            } else
                s->place = PlaceHolder(*session, it->id);
        }
    }
    s->teacher = UserHolder(*session, 1);
    s->receiver = UserHolder(*session, 1);
    return s;
}

QVector<Domain::ScheduleHolder>
DocxParser::pasreDocument(const QString &docx)
{
    Yb::Session *session = new Yb::Session(Yb::theSchema::instance(), theApp::instance().get_engine());
    QVector<Domain::ScheduleHolder> objects;

    Yb::ElementTree::ElementsPtr rows = getTableRowsFromXml(getXmlFromDocx(docx));
    Yb::ElementTree::Elements::iterator it_row = rows->begin();
    if (it_row != rows->end())
        ++it_row;
    else {
        qDebug() << "Table wasn't found..";
        return objects;
    }

    for (; it_row != rows->end(); ++it_row) {
        ScheduleHolder s = getRow(*it_row, session);
        if (!s->is_empty())
            objects.push_back(s);
    }
    qDebug() << objects.count();
    return objects;
}
