#ifndef DOCUMENTPARSER_H
#define DOCUMENTPARSER_H

#include <QString>
#include <QDomDocument>
#include <QFile>
#include <stdexcept>
#include <string>
#include <QVector>
#include <QtDebug>
#include <QtGui>


class Cell
{
public:
    QVector<QString> blocks_;
    //Cell();
    inline void addBlock(QString block)
    {
        blocks_.push_back(block);
    }

    inline int size()
    {
        return blocks_.size();
    }

    QString concatContent(QString separator = "\n")
    {
        QString rezult = "";
        foreach(QString text, blocks_)
            rezult += text + separator;
        return rezult;
    }

    bool hasSpecialCharacter()
    {
        foreach (QString content, blocks_)
        {
            if (content.contains(":") || content.contains("-"))
                return true;
        }
        return false;
    }
};

class ParserEx: public std::runtime_error
{
public:
    ParserEx(const std::string &ctx, const std::string &msg)
        : std::runtime_error(ctx + ": " + msg) {}
};

typedef QVector<Cell> Row;

class DocumentParser
{
    QDomNode table_;
    QDomNodeList rows_;

    QString parseBlock(QDomNode node);
public:
    DocumentParser(const QByteArray &content, const QString &doctype = "docx");
    QVector<Row> getRows();
};



#endif // DOCUMENTPARSER_H
