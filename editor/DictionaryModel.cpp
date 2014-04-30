#include "DictionaryModel.h"
#include "domain/Subject.h"
#include "domain/Activity.h"
#include "domain/User.h"
#include "domain/Place.h"
#include "domain/StudGroup.h"
#include <QDebug>
#include "orm/DomainObject.h"
#include <QTableView>
#include <QTextEdit>
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QApplication>
#include <QPainter>
#include <QMessageBox>

DictionaryModel::DictionaryModel(QString tbl_name, QObject *parent) :
    QAbstractItemModel(parent), m_session(theApp::instance().new_session()), m_tbl_name(tbl_name)
{
    clearModel();
    getColumnsNameFromSchema();
    if (m_tbl_name == trUtf8("T_SUBJECT"))
        fillModel<Domain::Subject>();
    else
    if (m_tbl_name == trUtf8("T_ACTIVITY"))
        fillModel<Domain::Activity>();
    else
    if (m_tbl_name == trUtf8("T_PLACE"))
        fillModel<Domain::Place>();
    else
    if (m_tbl_name == trUtf8("T_USER"))
        fillModel<Domain::User>();
    else
    if (m_tbl_name == trUtf8("T_STUD_GROUP"))
        fillModel<Domain::StudGroup>();
}

void DictionaryModel::getColumnsNameFromSchema()
{
    Yb::ElementTree::ElementPtr tbl_config = theApp::instance().get_table_config()->find_first(m_tbl_name);
    Yb::ElementTree::Elements::iterator it_config = tbl_config->children_.begin();
    for (;it_config != tbl_config->children_.end(); ++it_config)
    {
        int visible = (*it_config)->attrib_["visible"].toInt();
        if ( visible )
        {
            int col_width = (*it_config)->attrib_["width"].toInt();
            ColumnsInfo col_info;
            col_info.m_rus_name = (*it_config)->attrib_["name"];
            col_info.m_eng_name = (*it_config)->name_;
            (col_width) ? col_info.width = col_width : col_info.width = 100;
            m_columns_info.push_back(col_info);
        }
    }
}

QVariant DictionaryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    QVariant v;
    try
    {
        if (role  == Qt::DisplayRole)
        {
            v = m_objects.at( index.row() )->get(m_columns_info.at(index.column()).m_eng_name).as_string();
        }
    }
    catch (...)
    {
        v = QVariant(trUtf8("..."));
    }
    return v;
}

bool DictionaryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    try
    {
        if (index.isValid() && role == Qt::EditRole)
        {
            if (!m_objects.at( index.row() )->get_session())
                m_objects.at( index.row() )->save(*m_session);
            switch (m_objects.at( index.row() )->get(m_columns_info.at(index.column()).m_eng_name).get_type())
            {
                case Yb::Value::INTEGER:
                    m_objects.at( index.row() )->set(m_columns_info.at(index.column()).m_eng_name, value.toInt());
                    break;
                case Yb::Value::STRING:
                    m_objects.at( index.row() )->set(m_columns_info.at(index.column()).m_eng_name, value.toString());
                    break;
                case Yb::Value::DATETIME:
                    m_objects.at( index.row() )->set(m_columns_info.at(index.column()).m_eng_name, value.toDateTime());
                    break;
                default:
                    m_objects.at( index.row() )->set(m_columns_info.at(index.column()).m_eng_name, value.toString());
                    break;
            }
            emit dataChanged(index, index);
            return true;
        }
    }
    catch (const std::exception &ex)
    {
        QMessageBox::critical(0, QString::fromUtf8("Ошибка"),
                                 QString::fromUtf8(ex.what()),
                                 QMessageBox::Ok);
    }
    return false;
}

QVariant DictionaryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_columns_info.at(section).m_rus_name;
    return QVariant();
}

int DictionaryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_objects.count();
}

int DictionaryModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_columns_info.count();
}

QModelIndex DictionaryModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    return createIndex(row, column);
}

bool DictionaryModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(rows)
    beginInsertRows(index, position, position + rows - 1);
    m_objects.insert(position, rows, new Yb::DomainObject(m_session.get()->schema(), m_tbl_name));
    endInsertRows();
    return true;
}

bool DictionaryModel::removeRows(int position, int rows, const QModelIndex &index)
{
    try {
        m_objects.at(position)->delete_object();
        m_objects.remove(position, rows);
        if (!rowCount())
            insertRows(0, 1);
    }
    catch (const std::exception &ex) {
        std::string s(ex.what());
        size_t pos = s.find("Backtrace");
        if (pos != std::string::npos)
            s = s.substr(0, pos);
        QMessageBox::warning(0, QString::fromUtf8("Ошибка"),
                             QString::fromUtf8(s.c_str()),
                             QMessageBox::Ok);
        return false;
    }
    beginRemoveRows(index, position, position + rows - 1);
    endRemoveRows();
    return true;
}

QModelIndex DictionaryModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}

Qt::ItemFlags DictionaryModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void DictionaryModel::epicCommit()
{
    try {
        m_session->commit();
    }
    catch (const std::exception &ex) {
        QMessageBox::critical(0, QString::fromUtf8("Ошибка"),
                                 QString::fromUtf8(ex.what()),
                                 QMessageBox::Ok);
    }
}

void DictionaryModel::clearModel()
{
    beginRemoveRows(QModelIndex(), 0, m_objects.count());
    foreach(Yb::DomainObject* i, m_objects)
        delete i;
    m_columns_info.clear();
    endRemoveRows();
}

DictionaryDelegate::DictionaryDelegate(QObject *parent):
    QStyledItemDelegate (parent)
{
    QTableView *view = static_cast<QTableView *>(parent);
    DictionaryModel *model = static_cast<DictionaryModel *>(view->model());
    if (model)
    {
        QVector<ColumnsInfo>::const_iterator it = model->get_columns_info().begin();
        for(int i = 0; it != model->get_columns_info().end(); ++it)
            view->setColumnWidth(i, it->width);
    }
}

QWidget *DictionaryDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return new QTextEdit(parent);
}

void DictionaryDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    static_cast<QTextEdit *>(editor)->setText(index.model()->data(index).toString());
}

void DictionaryDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    model->setData(index, static_cast<QTextEdit *>(editor)->toPlainText());
}

bool DictionaryDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (me->button() == Qt::RightButton) {
            QMenu *menu = new QMenu();

            QAction *actAdd = new QAction(trUtf8("Добавить строку"), parent());
            actAdd->setData(QVariant(index.row()));
            connect(actAdd, SIGNAL(triggered()), SLOT(slotAddRow()));
            menu->addAction(actAdd);

            QAction *actRemove = new QAction(trUtf8("Удалить строку"), parent());
            actRemove->setData(QVariant(index.row()));
            connect(actRemove, SIGNAL(triggered()), SLOT(slotRemoveRow()));
            menu->addAction(actRemove);

            menu->exec(QCursor::pos());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void DictionaryDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    QRect rect(option.rect);
    editor->setGeometry(rect);
}

void DictionaryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    opt.text = "";
    const QWidget *widget = qobject_cast<QWidget*>(parent());
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
    QRect rct(option.rect.topLeft(), option.rect.size());
    QString text = index.data().toString().trimmed();
    painter->drawText(rct, Qt::AlignCenter, text);
}

void DictionaryDelegate::slotRemoveRow()
{
    int row = static_cast<QAction *>(sender())->data().toInt();
    QTableView *view = static_cast<QTableView *>(parent());
    view->model()->removeRows(row, 1);
}

void DictionaryDelegate::slotAddRow()
{
    int row = static_cast<QAction *>(sender())->data().toInt();
    QTableView *view = static_cast<QTableView *>(parent());
    view->model()->insertRows(row, 1);
}
