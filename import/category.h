#ifndef CATEGORY_H
#define CATEGORY_H

#include <QVector>
#include <QMultiMap>
#include <QString>
#include "classifier.h"
#include "documentparser.h"
#include "mainwindow.h"

#define _R QString::fromUtf8

class AbstractCategory
{
protected:
    int ruleCount_;
    QString name_;
public:
    virtual double calculateProbability(struct ColumnStatisticTmp&, QVector<Cell> &) =0;
    QString getName() const
    {
        return name_;
    }
    AbstractCategory(QString name, int ruleCount):
        ruleCount_(ruleCount)
      , name_(name)
    {}
};

class DowCategory: public AbstractCategory
{
public:
    DowCategory(): AbstractCategory(_R("День"), 3)
    {}
    double calculateProbability(struct ColumnStatisticTmp &statistic,
                             QVector<Cell> &column);
};

class SubjectCategory: public AbstractCategory
{
public:
    SubjectCategory(): AbstractCategory(_R("Дисциплина"), 3)
    {}
    double calculateProbability(struct ColumnStatisticTmp &statistic,
                             QVector<Cell> &column);
};

class PlaceCategory: public AbstractCategory
{
public:
    PlaceCategory(): AbstractCategory(_R("Место"), 4)
    {}
    double calculateProbability(struct ColumnStatisticTmp &statistic,
                             QVector<Cell> &column);
};

class DateCategory: public AbstractCategory
{
public:
    DateCategory(): AbstractCategory(_R("Дата"), 3)
    {}
    double calculateProbability(struct ColumnStatisticTmp &statistic,
                             QVector<Cell> &column);
};

class TimeCategory: public AbstractCategory
{
public:
    TimeCategory(): AbstractCategory(_R("Время"), 3)
    {}
    double calculateProbability(struct ColumnStatisticTmp &statistic,
                             QVector<Cell> &column);
};

class CourseCategory: public AbstractCategory
{
public:
    CourseCategory(): AbstractCategory(_R("Курс"), 2)
    {}
    double calculateProbability(struct ColumnStatisticTmp &statistic,
                             QVector<Cell> &column);
};

class CategoryFactory
{
    QVector<AbstractCategory*> category_;
public:
    CategoryFactory(){}
    void addCategory(AbstractCategory *category);
    QMultiMap<double, AbstractCategory*> calculateProbability(struct ColumnStatisticTmp &statistic,
                                             QVector<Cell> &column);
};

#endif // CATEGORY_H
