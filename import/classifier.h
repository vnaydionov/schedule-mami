#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "category.h"
#include "documentparser.h"
#include <QVector>
#include <QString>
#include <QStringList>
#include <QtDebug>
#include "mainwindow.h"
#include <QMultiMap>
#include <algorithm>

typedef struct ColumnStatisticTmp
{
    ColumnStatisticTmp():
        words(0)
      , symbols(0)
      , numbers(0)
      ,  maxStringSize(0)
      , minStringSize(1000)
      , hasComa(false)
      , hasCodon(false)
      , hasDash(false)
    {}
    void normalize(int count)
    {
        words = words/count;
        symbols = symbols/count;
        numbers = numbers/count;
    }

    double words;
    double symbols;
    double numbers;
    int maxStringSize;
    int minStringSize;
    bool hasComa;
    bool hasCodon;
    bool hasDash;
} ColumnStatistic;

class CategoryFactory;

class Classifier
{
    CategoryFactory *categories_;
    QVector<Row> source_table_;
    QVector< QVector<QString> > rows_;
    void normalize(QVector<Row> &rows);
    void calculateCharacteristic(const Cell &cell, ColumnStatistic &statistic);
    QString analizeStatistic(ColumnStatistic statistic);
public:
    static QStringList categories;
    Classifier(QVector<Row> &rows, CategoryFactory *categories);
    QVector<QString> suggestionParse();
    inline QVector< QVector<QString> > getRows() const
    {
        return rows_;
    }
};



#endif // CLASSIFIER_H
