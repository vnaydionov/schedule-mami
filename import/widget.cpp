#include "widget.h"

HeaderWidget::HeaderWidget(QWidget *parent) :
    QWidget(parent)
{
    QStringList categories;
    categories << "Время" << "День" << "Дата" << "Предмет" << "Курс" << "Место";
    QComboBox *combo_box = new QComboBox(this);
    combo_box->addItems(categories);
}
