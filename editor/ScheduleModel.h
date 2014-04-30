#ifndef SCHEDULE_MODEL_H
#define SCHEDULE_MODEL_H

#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QVector>
#include <memory>
#include "domain/Schedule.h"

enum Role {
    CheckStateRole = 1,
    ScheduleObjectRole
    };

enum CheckState {
    NotConfirmed = 0,
    Confirmed
};

enum ColumnName {
    StateColumn = 0,
    DowColumn,
    TimeColumn,
    SubjectColumn,
    GroupColumn,
    DateColumn,
    PlaceColumn,
    TeacherColumn
};

namespace Domain {
class Schedule;
}

namespace Yb {
class Session;
}

Q_DECLARE_METATYPE(Domain::ScheduleHolder)

class ScheduleModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ScheduleModel(QObject *parent=0);
    ~ScheduleModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    void fill(const QVector<Domain::ScheduleHolder> &objects);
    void epicCommit();
private:
    std::auto_ptr<Yb::Session> m_session;
    QVector<Domain::ScheduleHolder> m_objects;
    QList<QVariant> m_headersData;
}; // class ScheduleModel

class ScheduleDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ScheduleDelegate(QObject *parent = 0);
    void paint (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;
protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index);
    bool eventFilter(QObject *object, QEvent *event);
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private slots:
    void slotRemoveRow();
    void slotAddRow();
    void slotOpenDictEditor();
    void slotLoadScheduleFromDocx(const QString &fileName);
    void slotSaveToPdf();
private:
    void drawPixmap(QPainter *painter, const QRect &rct, int type) const;
    void settingsView();
    QStringList dowsList() const;
    QStringList timeList() const;
    QStringList placesList() const;
    QStringList teachersList() const;
    QStringList datesList() const;
    QStringList subjectsList(const QModelIndex &index) const;
    void groupsEditor(const QModelIndex &index) const;
};// class ScheduleDelegate

class HeaderViewEventFilter: public QObject {
    Q_OBJECT
public:
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // SCHEDULE_MODEL_H
