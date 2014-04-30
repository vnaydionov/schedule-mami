#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <vector>
#include <string>
#include "documentparser.h"
#include <QVector>
#include "classifier.h"
#include <QStringList>
#include <QComboBox>
#include "mainwindow.h"
#include <QtDebug>
#include "category.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    QVector< QVector<QString> > table_;
public:
    explicit TableModel(QVector<Row> table, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        Q_UNUSED(parent);
        return table_.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        Q_UNUSED(parent);
        return table_.at(0).size();
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
        theFlags |= Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        if (index.isValid() && index.row() == 0)
            theFlags |= Qt::ItemIsEditable;
        return theFlags;
    }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVector<QString> getHead() const
    {
        return table_.at(0);
    }

signals:
    
public slots:
    
};

#endif // TABLEMODEL_H
