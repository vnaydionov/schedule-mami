#include "XmlConfigModel.h"
#include <QDebug>
#include <QTableView>
#include <QTextEdit>
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QApplication>
#include <QPainter>
#include <QCheckBox>

XmlConfigModel::XmlConfigModel(QString tbl_name, QObject *parent) :
    QAbstractItemModel(parent), m_tbl_name(tbl_name)
{
    Yb::ElementTree::ElementPtr tbl_config = theApp::instance().get_table_config()->find_first(m_tbl_name);
    Yb::ElementTree::Elements::iterator it_config = tbl_config->children_.begin();
    for (;it_config != tbl_config->children_.end(); ++it_config)
    {
        RowInfo col_info;
        col_info.m_visible = (*it_config)->attrib_["visible"].toInt() ? true : false;
        col_info.m_width = (*it_config)->attrib_["width"].toInt();
        col_info.m_rus_name = (*it_config)->attrib_["name"];
        col_info.m_eng_name = (*it_config)->name_;
        m_columns_info.push_back(col_info);
    }
    m_feature_name << trUtf8("Название") << trUtf8("Показывать") << trUtf8("Ширина");
}

XmlConfigModel::~XmlConfigModel()
{
    try
    {
        Yb::ElementTree::ElementPtr tbl_config = theApp::instance().get_table_config()->find_first(m_tbl_name);
        for(int i = 0; i < m_columns_info.size(); i++)
        {
            try
            {
                Yb::ElementTree::ElementPtr ptr = (*tbl_config).find_first(m_columns_info.at(i).m_eng_name);
                qDebug() <<  m_columns_info.at(i).m_visible
                         <<  m_columns_info.at(i).m_width
                         << m_columns_info.at(i).m_rus_name;
                if (ptr->has_attr("visible"))
                    ptr->attrib_["visible"] = Yb::Value( (m_columns_info.at(i).m_visible ? 1 : 0 ) ).as_string();
                if (ptr->has_attr("width"))
                    ptr->attrib_["width"] = Yb::Value(m_columns_info.at(i).m_width).as_string();
                if (ptr->has_attr("name"))
                    ptr->attrib_["name"] = Yb::Value(m_columns_info.at(i).m_rus_name).as_string();
                qDebug() <<  ptr->attrib_["visible"]
                         <<  ptr->attrib_["width"]
                         <<  ptr->attrib_["name"];
            }
            catch (...) { }
        }
    }
    catch (...) { }
}

QVariant XmlConfigModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    QVariant v;
    if (role  == Qt::DisplayRole)
    {
        if (index.row() == 0)
            v = m_columns_info.at(index.column()).m_rus_name;
        else
        if (index.row() == 1)
            v = (m_columns_info.at(index.column()).m_visible) ? trUtf8("Да") : trUtf8("Нет") ;
        else
        if (index.row() == 2)
            v = m_columns_info.at(index.column()).m_width;
    }
    return v;
}

bool XmlConfigModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        if (index.row() == 0)
            m_columns_info[ index.column() ].m_rus_name = value.toString();
        else
        if (index.row() == 1)
            m_columns_info[ index.column() ].m_visible = value.toString().compare(trUtf8("Да")) ? false : true;
        if (index.row() == 2)
            m_columns_info[ index.column() ].m_width = value.toInt();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

QVariant XmlConfigModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_columns_info.at(section).m_eng_name;
    if (orientation == Qt::Vertical && role == Qt::DisplayRole)
        return m_feature_name.at(section);
    return QVariant();
}

int XmlConfigModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

int XmlConfigModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_columns_info.count();
}

QModelIndex XmlConfigModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    return createIndex(row, column);
}

QModelIndex XmlConfigModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

Qt::ItemFlags XmlConfigModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

XmlConfigDelegate::XmlConfigDelegate(QObject *parent):
    QStyledItemDelegate (parent)
{
    QTableView *view = static_cast<QTableView *>(parent);
    view->setColumnWidth(0, 70);
    for (int i = 1; i < view->model()->columnCount(); i++)
        view->setColumnWidth(i, 160);
}

QWidget *XmlConfigDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    if (index.row() == 1)
        return new QCheckBox(parent);
    return new QTextEdit(parent);
}

void XmlConfigDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.row() == 1)
        static_cast<QCheckBox *>(editor)->setChecked( (index.model()->data(index).toString().compare(trUtf8("Да")) ? false : true) );
    else
        static_cast<QTextEdit *>(editor)->setText(index.model()->data(index).toString());
}

void XmlConfigDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.isValid())
    {
        if (index.row() == 1)
        {
            QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
            model->setData(index, checkBox->isChecked() ? trUtf8("Да") : trUtf8("Нет"));
        }
        else
        {
            model->setData(index, static_cast<QTextEdit *>(editor)->toPlainText());
        }
    }
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}

void XmlConfigDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    QRect rect(option.rect);
    editor->setGeometry(rect);
}
