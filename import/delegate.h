#ifndef DELEGATE_H
#define DELEGATE_H

#include <QItemDelegate>
#include <QComboBox>
#include <QStringList>
#include "mainwindow.h"
#include <QtDebug>

class TableHeaderDelegate : public QItemDelegate
{
public:
    TableHeaderDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const;
};

#endif // DELEGATE_H
