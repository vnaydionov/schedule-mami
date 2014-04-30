#ifndef DOCXPARSER_H
#define DOCXPARSER_H

#include <QList>
#include <QString>
#include "domain/Schedule.h"
#include <QPair>

class DocxParser
{
    static Yb::ElementTree::ElementPtr getXmlFromDocx(const QString &docx);
    static Yb::ElementTree::ElementsPtr getTableRowsFromXml(Yb::ElementTree::ElementPtr root);
    static Domain::ScheduleHolder getRow(const Yb::ElementTree::ElementPtr &e, Yb::Session *session);
public:
    static QVector<Domain::ScheduleHolder> pasreDocument(const QString &docx);
};



#endif // DOCXPARSER_H
