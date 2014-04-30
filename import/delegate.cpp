#include "delegate.h"
#include <QLineEdit>

TableHeaderDelegate::TableHeaderDelegate(QObject *parent)
{
}

QWidget*
TableHeaderDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    QComboBox *combo_box = new QComboBox(parent);
    QStringList categories;
    categories << _R("Дата") << _R("Время") << _R("Место") << _R("Дисциплина") << _R("День") << _R("Курс");
    combo_box->addItems(categories);
    //combo_box->view()->setEditTriggers(QAbstractItemView::AllEditTriggers);
    return combo_box;
}

void
TableHeaderDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    QComboBox *combo_box = qobject_cast<QComboBox *>(editor);
    combo_box->setCurrentIndex(index.row());
}

void
TableHeaderDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
    QComboBox *combo_box = qobject_cast<QComboBox *>(editor);
    QVariant val = combo_box->currentText();
    model->setData(index, val, Qt::EditRole);
}

