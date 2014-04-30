#include "classifier.h"

Classifier::Classifier(QVector<Row> &rows, CategoryFactory *categories):
    categories_(categories)
  , source_table_(rows)
{
    normalize(rows);
}

void
Classifier::normalize(QVector<Row> &rows)
{
    foreach (Row row, rows){
        QVector<QString> new_row;
        foreach(Cell cell, row)
        {
           new_row.push_back(cell.concatContent());
        }
        rows_.push_back(new_row);
    }
}

QVector<QString>
Classifier::suggestionParse()
{
    QVector<QString> headerList;
    for (int i = 0; i < source_table_.at(0).size(); ++i)
    {
        QVector<Cell> currentColumn;
        ColumnStatistic statistic;
        for (int j = 0; j < source_table_.size(); ++j)
        {
            Cell currentCell = source_table_.at(j).at(i);
            calculateCharacteristic(currentCell, statistic);
            currentColumn.push_back(currentCell);
        }
        statistic.normalize(source_table_.size());
        QMultiMap<double, AbstractCategory*> rez = categories_->calculateProbability(statistic, currentColumn);
        QString header = (rez.end()-1).value()->getName();
        headerList.push_back(header);
    }
    return headerList;
}

void
Classifier::calculateCharacteristic(const Cell &cell, ColumnStatistic &statistic)
{
    foreach (QString str, cell.blocks_)
    {
        if (str.size() > statistic.maxStringSize)
            statistic.maxStringSize = str.size();
        if (str.size() < statistic.minStringSize)
            statistic.minStringSize = str.size();
        QStringList words = str.split(" ");
        statistic.words += words.size();
        foreach (QChar ch, str)
        {
            if (ch == QChar(','))
                statistic.hasComa = true;
            if (ch == QChar(':'))
                statistic.hasCodon = true;
            if (ch == QChar('-'))
                statistic.hasDash = true;
            if (ch.isDigit())
                statistic.numbers++;
            if (ch.isLetter())
                statistic.symbols++;
        }
    }
}

QString
Classifier::analizeStatistic(ColumnStatistic statistic)
{
    qDebug() << "Statistic: ";
    qDebug() << "words: " << statistic.words;
    qDebug() << "symbols: " << statistic.symbols;
    qDebug() << "numbers: " << statistic.numbers;
    qDebug() << "max string size: " << statistic.maxStringSize;
    qDebug() << "min string size: " << statistic.minStringSize;
    qDebug() << "has coma: " << statistic.hasComa;

    if (statistic.symbols == 0. && statistic.words == 1. && statistic.hasComa
            && statistic.numbers == 4.)
        return _R("Место");
    if (statistic.words < 2. && statistic.maxStringSize < 12.
            && statistic.numbers == 0)
        return _R("День");
    if (statistic.words == 1. && statistic.symbols == 0.)
        return _R("Курс");
    if (statistic.symbols <= 1. && statistic.numbers > 0.)
        return _R("Время");
    if (statistic.symbols > 2. && statistic.words > 1.
            && statistic.numbers < 1.)
        return _R("Дисциплина");

    return _R("Неопознано");
}


