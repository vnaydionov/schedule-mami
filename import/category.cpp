#include "category.h"

double
DowCategory::calculateProbability(ColumnStatistic &statistic, QVector<Cell> &column)
{
    double prob = 0;
    if (statistic.words < 2.)
        prob += 1;
    if (statistic.maxStringSize < 12.)
        prob += 1;
    if (statistic.numbers == 0)
        prob += 1;
    return prob/ruleCount_;
}

double
PlaceCategory::calculateProbability(ColumnStatisticTmp &statistic, QVector<Cell> &column)
{
    double prob = 0;
    if (statistic.symbols == 0.)
        prob += 1;
    if (statistic.words == 1.)
        prob += 1;
    if (statistic.hasComa)
        prob += 1;
    if (statistic.numbers == 4.)
        prob += 1;
    return prob/ruleCount_;
}

double
SubjectCategory::calculateProbability(ColumnStatisticTmp &statistic, QVector<Cell> &column)
{
    double prob = 0;
    if (statistic.symbols > 2.)
        prob += 1;
    if (statistic.words > 1.)
        prob += 1;
    if (statistic.numbers < 1.)
        prob += 1;
    return prob/ruleCount_;
}

double
CourseCategory::calculateProbability(ColumnStatisticTmp &statistic, QVector<Cell> &column)
{
    double prob = 0;
    if (statistic.words == 1.)
        prob += 1;
    if (statistic.symbols == 0.)
        prob += 1;
    return prob/ruleCount_;
}

double
DateCategory::calculateProbability(ColumnStatisticTmp &statistic, QVector<Cell> &column)
{
    double prob = 0;
    if (statistic.symbols <= 1.)
        prob += 1;
    if (statistic.numbers > 0.)
        prob += 1;
    if (statistic.hasDash || statistic.hasCodon)
        prob += 1;

    foreach(Cell cell, column)
    {
        if (cell.hasSpecialCharacter()){
            foreach(QString str, cell.blocks_)
            {
                QStringList l = str.split("-");
                if (l.count() > 1 && statistic.symbols <= 1.)
                {
                    QStringList num = l[0].split(".");
                    if (num[1].toInt() > 12)
                    {
                        prob -= 1;
                        break;
                    }
                }
            }
        }
    }

    return prob/ruleCount_;
}

double
TimeCategory::calculateProbability(ColumnStatisticTmp &statistic, QVector<Cell> &column)
{
    double prob = 0;
    if (statistic.symbols <= 1.)
        prob += 1;
    if (statistic.numbers > 0.)
        prob += 1;
    if (statistic.hasDash || statistic.hasCodon)
        prob += 1;
    return prob/ruleCount_;
}

void
CategoryFactory::addCategory(AbstractCategory *category)
{
    category_.push_back(category);
}

QMultiMap<double, AbstractCategory*>
CategoryFactory::calculateProbability(ColumnStatistic &statistic, QVector<Cell> &column)
{
    QMultiMap<double, AbstractCategory*> probability;
    foreach (AbstractCategory *category, category_)
    {
        double prob = category->calculateProbability(statistic, column);
        probability.insert(prob, category);
    }
    return probability;
}
