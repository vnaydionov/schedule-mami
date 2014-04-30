#include "tablemodel.h"

using namespace std;

TableModel::TableModel(QVector<Row> table, QObject *parent) :
    QAbstractTableModel(parent)
{
    table.remove(0);

    SharedObjSingleton::instance().setRows(table);

    CategoryFactory *factory = new CategoryFactory();
    AbstractCategory *category = new DowCategory();
    factory->addCategory(category);
    category = new SubjectCategory();
    factory->addCategory(category);
    category = new DateCategory();
    factory->addCategory(category);
    category = new TimeCategory();
    factory->addCategory(category);
    category = new PlaceCategory();
    factory->addCategory(category);
    category = new CourseCategory();
    factory->addCategory(category);

    Classifier cf(table, factory);
    table_ = cf.getRows();
    table_.push_front(cf.suggestionParse());
}

QVariant
TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() > table_.size()
            || index.column() > table_.at(0).size())
        return QVariant::Invalid;

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        QVector<QString> row = table_.at(index.row());
        return row.at(index.column());
    }
    return QVariant::Invalid;
}

QVariant
TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role != Qt::DisplayRole && role != Qt::EditRole
            && orientation != Qt::Vertical)
        return QVariant();
    return section + 1;
}

bool
TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() == 0)
    {
        table_[0][index.column()] = value.toString();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

