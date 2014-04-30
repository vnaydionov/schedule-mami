#include "ScheduleModel.h"

#include <QList>
#include "domain/Schedule.h"
#include "domain/ScheduleGroup.h"
#include "domain/ScheduleActivity.h"
#include "domain/ScheduleDate.h"
#include "App.h"
#include <QApplication>
#include <QPainter>
#include <QPushButton>
#include <QTableView>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include "ReferenceEditor.h"
#include <QInputDialog>
#include "ScheduleDatesDialog.h"
#include "ActivityDialog.h"
#include "StudGroupDialog.h"
#include "ScheduleDatesDialog.h"
#include <QComboBox>
#include <QHeaderView>
#include "DocxParser.h"
#include "Utils.h"
#include <QMessageBox>


ScheduleModel::ScheduleModel(QObject *parent):
    QAbstractItemModel(parent),
    m_session(theApp::instance().new_session())
{
    m_headersData   << trUtf8("")
                    << trUtf8("День недели")
                    << trUtf8("Время")
                    << trUtf8("Дисциплина")
                    << trUtf8("Группы")
                    << trUtf8("Дата")
                    << trUtf8("Аудитория")
                    << trUtf8("Преподаватель");
}

ScheduleModel::~ScheduleModel()
{ }

QVariant ScheduleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    QVariant v;
    if (role  == Qt::DisplayRole || role == CheckStateRole) {
        try {
            if (index.column() == StateColumn)
                v = m_objects.at(index.row())->checked.value();
            if (index.column() == DowColumn)
                v = m_objects.at(index.row())->dow.value();
            if (index.column() == TimeColumn) {
                QString startTime, endTime;
                if (m_objects.at(index.row())->start_time.value()-1000 > 0)
                    startTime = QString::number(m_objects.at(index.row())->start_time.value()).insert(2, ":");
                else
                    startTime = QString::number(m_objects.at(index.row())->start_time.value()).insert(1, ":");
                if (m_objects.at(index.row())->end_time.value()-1000 > 0)
                    endTime = QString::number(m_objects.at(index.row())->end_time.value()).insert(2, ":");
                else
                    endTime = QString::number(m_objects.at(index.row())->end_time.value()).insert(1, ":");
                v = startTime + trUtf8("-") + endTime;
                qDebug() << v;
            }
            if (index.column() == SubjectColumn) {
                Yb::ManagedList<Domain::ScheduleActivity>::iterator it = m_objects.at(index.row())->activities.begin();
                QString text = m_objects.at(index.row())->subject->name.value();
                for(;it != m_objects.at(index.row())->activities.end(); ++it) {
                    if (!text.isEmpty())
                        text += ",\n";
                    text += it->activity->name;
                }
                v = text;
            }
            if (index.column() == GroupColumn) {
                Yb::ManagedList<Domain::ScheduleGroup>::iterator it = m_objects.at(index.row())->groups.begin();
                QString text;
                for(;it != m_objects.at(index.row())->groups.end(); ++it) {
                    if (!text.isEmpty())
                        text += ",\n";
                    text += it->group->name_code;
                }
                v = text;
            }
            if (index.column() == DateColumn) {
                QString text,
                        includes,
                        excludes;
                text += m_objects.at(index.row())->start_dt.value().toString("dd.MM.yy");
                text += "-" + m_objects.at(index.row())->end_dt.value().toString("dd.MM.yy");
                Yb::ManagedList<Domain::ScheduleDate>::iterator it = m_objects.at(index.row())->dates.begin();
                for(;it != m_objects.at(index.row())->dates.end(); ++it) {
                    if (it->item_type.value() == "include") {
                        if (includes.isEmpty())
                            includes = trUtf8("\nВключая: ");
                        includes += it->dt.value().toString("dd.MM.yy");
                    } else {
                        if (excludes.isEmpty())
                            excludes = trUtf8("\nКроме: ");
                        excludes += it->dt.value().toString("dd.MM.yy");
                    }
                }
                v = text + includes + excludes;
            }
            if (index.column() == PlaceColumn)
                v = m_objects.at(index.row())->place->name.value();
            if (index.column() == TeacherColumn)
                v = m_objects.at(index.row())->teacher->name.value();
        } catch(...) {
            v = QVariant(trUtf8("..."));
        }
    }
    if (role  == ScheduleObjectRole)
        v = QVariant::fromValue(m_objects.at(index.row()));
    return v;
}

bool ScheduleModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        if (index.column() == DowColumn)
            m_objects.at(index.row())->dow = value.toString();
        if (index.column() == TimeColumn) {
            QString tmp = value.toString();
            tmp.remove(':');
            m_objects.at(index.row())->start_time = tmp.left(tmp.indexOf("-")).toInt();
            m_objects.at(index.row())->end_time = tmp.right(tmp.length() - tmp.indexOf("-")-1).toInt();
        }
        if (index.column() == PlaceColumn) {
            Domain::Place p = Yb::query<Domain::Place>(*m_session)
                        .filter_by(Domain::Place::c.name == value.toString())
                        .one();
            Domain::PlaceHolder place(*m_session, p.id);
            m_objects.at(index.row())->place = place;
        }
        if (index.column() == TeacherColumn) {
            Domain::User usr = Yb::query<Domain::User>(*m_session)
                        .filter_by(Domain::User::c.name == value.toString())
                        .one();
            Domain::UserHolder teacher(*m_session, usr.id);
            m_objects.at(index.row())->teacher = teacher;
            m_objects.at(index.row())->receiver = teacher;
        }
        // For other columns this method will call only for emit signal dataChanged()
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags ScheduleModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant ScheduleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_headersData.value(section);

    return QVariant();
}

QModelIndex ScheduleModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex ScheduleModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}

int ScheduleModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_objects.count();
}

int ScheduleModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_headersData.count();
}

bool ScheduleModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(rows)
    beginInsertRows(index, position, position+rows-1);
    m_objects.insert(position, rows, Domain::ScheduleHolder(*m_session));
    endInsertRows();
    return true;
}

void ScheduleModel::fill(const QVector<Domain::ScheduleHolder> &objects)
{
    beginRemoveRows(QModelIndex(), 0, m_objects.count());
    m_objects.clear();
    endRemoveRows();

    if (objects.isEmpty())
        return;
    m_session.reset(objects.at(0)->get_session());
    beginInsertRows(QModelIndex(), 0, objects.count());
    foreach (Domain::ScheduleHolder row, objects)
        m_objects.push_back(row);
    endInsertRows();
}

void ScheduleModel::epicCommit()
{
    m_session->commit();
}

bool ScheduleModel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(index, position, position+rows-1);
    m_objects.at(position)->delete_object();
    m_objects.remove(position, rows);
    endRemoveRows();
    return true;
}

ScheduleDelegate::ScheduleDelegate(QObject *parent):
    QStyledItemDelegate (parent)
{
    settingsView();
}

void ScheduleDelegate::settingsView()
{
    QTableView *view = static_cast<QTableView *>(parent());
    try {
        Yb::ElementTree::ElementPtr tbl_config = theApp::instance().get_table_config()->find_first("T_SCHEDULE");
        view->setColumnWidth(StateColumn, 45);  // Image
        view->setColumnWidth(DowColumn, tbl_config->find_first("DOW")->attrib_["width"].toInt());
        view->setColumnWidth(TimeColumn, tbl_config->find_first("TIME")->attrib_["width"].toInt());
        view->setColumnWidth(SubjectColumn, tbl_config->find_first("SUBJECT")->attrib_["width"].toInt());
        view->setColumnWidth(GroupColumn, tbl_config->find_first("GROUPS")->attrib_["width"].toInt());
        view->setColumnWidth(DateColumn, tbl_config->find_first("DATE")->attrib_["width"].toInt());
        view->setColumnWidth(PlaceColumn, tbl_config->find_first("PLACE")->attrib_["width"].toInt());
        view->setColumnWidth(TeacherColumn, tbl_config->find_first("TEACHER")->attrib_["width"].toInt());
    } catch(const std::runtime_error &err) {
        QMessageBox::warning(0, trUtf8("Предупреждение"), trUtf8("Файл с настройками таблицы поврежден.."), QMessageBox::Ok);
        qDebug() << err.what();
    }

    view->verticalHeader()->setDefaultSectionSize(90);
    view->viewport()->installEventFilter(this);
    //HeaderViewEventFilter *headerEventFilter = new HeaderViewEventFilter();
    //view->horizontalHeader()->viewport()->installEventFilter(headerEventFilter);
}


void ScheduleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    opt.text = "";
    const QWidget *widget = qobject_cast<QWidget*>(parent());
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

    QRect rct(option.rect.topLeft(), option.rect.size());
    if (index.column() == 0) {
        if (index.data().toInt() == Confirmed)
            drawPixmap(painter, rct, Confirmed);
        else
            drawPixmap(painter, rct, NotConfirmed);
    } else {
        QString text = index.data().toString().trimmed();
        painter->drawText(rct, Qt::AlignCenter, text);
    }
}

void ScheduleDelegate::drawPixmap(QPainter *painter, const QRect &rct, int type) const
{
    QPixmap p;
    // Draw frame with confirm icon
    if (type == Confirmed)
        p = QPixmap(":/images/confirmed.png");
    // Draw frame with not confirm icon
    if (type == NotConfirmed)
        p = QPixmap(":/images/not_confirmed.png");
    QApplication::style()->drawItemPixmap(painter, rct, Qt::AlignCenter, p);
}

bool ScheduleDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (me->button() == Qt::RightButton) {
            QMenu *menu = new QMenu();

            QAction *actAdd = new QAction(QIcon(":/images/add.png"), trUtf8("Добавить строку"), parent());
            actAdd->setData(QVariant(index.row()));
            connect(actAdd, SIGNAL(triggered()), SLOT(slotAddRow()));
            menu->addAction(actAdd);

            QAction *actRemove = new QAction(QIcon(":/images/remove.png"), trUtf8("Удалить строку"), parent());
            actRemove->setData(QVariant(index.row()));
            connect(actRemove, SIGNAL(triggered()), SLOT(slotRemoveRow()));
            menu->addAction(actRemove);

            menu->exec(QCursor::pos());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void ScheduleDelegate::slotRemoveRow()
{
    int row = static_cast<QAction *>(sender())->data().toInt();
    QTableView *view = static_cast<QTableView *>(parent());
    view->model()->removeRows(row, 1);
}

void ScheduleDelegate::slotAddRow()
{
    int row = static_cast<QAction *>(sender())->data().toInt();
    QTableView *view = static_cast<QTableView *>(parent());
    view->model()->insertRows(row, 1);
}

void ScheduleDelegate::slotOpenDictEditor()
{
    ReferenceEditor *d = new ReferenceEditor();
    d->show();
}

void ScheduleDelegate::slotLoadScheduleFromDocx(const QString &fileName)
{
    QTableView *view = static_cast<QTableView *>(parent());
    static_cast<ScheduleModel *>(view->model())->fill(DocxParser::pasreDocument(fileName));
}

void ScheduleDelegate::slotSaveToPdf()
{
    SchedulePrinter::printToPDF(static_cast<QTableView *>(parent()));
}

QWidget *ScheduleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    if (index.column() == StateColumn)
        return NULL;
    QWidget *w = NULL;
    if (index.column() == GroupColumn)
        w = new StudGroupDialog(index.data(ScheduleObjectRole).value<Domain::ScheduleHolder>());
    else if (index.column() == SubjectColumn)
        w =  new ActivityDialog(index.data(ScheduleObjectRole).value<Domain::ScheduleHolder>());
    else if (index.column() == DateColumn)
        w =  new ScheduleDatesDialog(index.data(ScheduleObjectRole).value<Domain::ScheduleHolder>());
    else {
        w = new QComboBox(parent);
        w->setStyleSheet("border: 3px solid rgb(95, 95, 95); "
                         "border-radius: 5px;"
                         "padding: 1px 1px 1px 1px;"
                         "margin: 0 0 0 0; ");
    }
    return w;
}

void ScheduleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() != GroupColumn &&
        index.column() != SubjectColumn &&
        index.column() != DateColumn)
    {
        QComboBox *cb = static_cast<QComboBox *>(editor);
        if (index.column() == DowColumn)
            cb->addItems(dowsList());
        if (index.column() == TimeColumn)
            cb->addItems(timeList());
        if (index.column() == PlaceColumn)
            cb->addItems(placesList());
        if (index.column() == TeacherColumn)
            cb->addItems(teachersList());
        if (cb->findText(index.data().toString()))
            cb->addItem(cb->currentText());

        if (cb->findText(index.data().toString()) == -1)
            cb->addItem(index.data().toString());
        cb->setCurrentIndex(cb->findText(index.data().toString()));
    }
}

void ScheduleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // Data for columns Groups, Subject and Dates is setting in dialogs
    if (index.column() == GroupColumn ||
        index.column() == SubjectColumn ||
        index.column() == DateColumn)
        // Call setData for emit signal dataChanget for this index
        // It's needed for update view
        model->setData(index, QVariant());
    else
        model->setData(index, static_cast<QComboBox *>(editor)->currentText());
}

QStringList ScheduleDelegate::dowsList() const
{
    QStringList items;
    items << trUtf8("Понедельник")
          << trUtf8("Вторник")
          << trUtf8("Среда")
          << trUtf8("Четверг")
          << trUtf8("Пятница")
          << trUtf8("Суббота")
          << trUtf8("Воскресенье");
    return items;
}

QStringList ScheduleDelegate::timeList() const
{
    QStringList items;
    items << trUtf8("9:00-10:30")
          << trUtf8("10:40-12:20")
          << trUtf8("12:25-13:55")
          << trUtf8("14:00-15:30");
    return items;
}

QStringList ScheduleDelegate::placesList() const
{
    QStringList items;
    std::auto_ptr<Yb::Session> session = theApp::instance().new_session();
    Yb::DomainResultSet<Domain::Place> rs = Yb::query<Domain::Place>(*session).all();
    Yb::DomainResultSet<Domain::Place>::iterator it = rs.begin();
    for (; it != rs.end(); ++it)
        items << it->name;
    return items;
}

QStringList ScheduleDelegate::teachersList() const
{
    QStringList items;
    std::auto_ptr<Yb::Session> session = theApp::instance().new_session();
    Yb::DomainResultSet<Domain::User> rs = Yb::query<Domain::User>(*session).all();
    Yb::DomainResultSet<Domain::User>::iterator it = rs.begin();
    for (; it != rs.end(); ++it)
        items << it->name;
    return items;
}

bool ScheduleDelegate::eventFilter(QObject *object, QEvent *event)
{
    if (static_cast<QTableView *>(parent())->model()->rowCount())
        return QStyledItemDelegate::eventFilter(object, event);
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (me->button() == Qt::RightButton) {
            QMenu *menu = new QMenu();

            QAction *actAdd = new QAction(QIcon(":/images/add.png"), trUtf8("Добавить строку"), parent());
            actAdd->setData(QVariant(0));
            connect(actAdd, SIGNAL(triggered()), SLOT(slotAddRow()));
            menu->addAction(actAdd);

            menu->exec(QCursor::pos());
            return true;
        }
    }
    return QStyledItemDelegate::eventFilter(object, event);
}

void ScheduleDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    QRect rect(option.rect);
    editor->setGeometry(rect);
}

bool HeaderViewEventFilter::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (me->button() == Qt::RightButton) {
            QMenu *menu = new QMenu();

            QAction *actAdd = new QAction(QIcon(":/images/filter.png"), trUtf8("Добавить фильтр"), parent());
            actAdd->setData(QVariant(0));
            connect(actAdd, SIGNAL(triggered()), SLOT(slotCreateFilter()));
            menu->addAction(actAdd);

            menu->exec(QCursor::pos());
            return true;
        }
    }
    return false;
}

