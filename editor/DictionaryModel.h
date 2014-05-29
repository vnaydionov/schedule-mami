#ifndef DICTIONARYMODEL_H
#define DICTIONARYMODEL_H

#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QStringList>
#include <QVector>
#include "orm/domain_object.h"
#include "orm/data_object.h"
#include "App.h"

struct ColumnsInfo
{
    QString m_eng_name;
    QString m_rus_name;
    int width;
};

class DictionaryModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DictionaryModel(QString tbl_name, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    QModelIndex parent(const QModelIndex &child) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    const QVector<ColumnsInfo> &get_columns_info() { return m_columns_info; }
    void epicCommit();
    void clearModel();
private:
    void getColumnsNameFromSchema();
    void prepareTable();
    template<class T>
    void fillModel()
    {
        typename Yb::DomainResultSet<T> rs = Yb::query<T>(*m_session).all();
        typename Yb::DomainResultSet<T>::iterator it_rs = rs.begin();
        for(;it_rs != rs.end(); ++it_rs)
            m_objects.push_back( new Yb::DomainObject(*m_session, T::get_table_name(), it_rs->id));
    }
    std::auto_ptr<Yb::Session> m_session;
    QString m_tbl_name;
    QVector< Yb::DomainObject *> m_objects;
    QVector<ColumnsInfo> m_columns_info;
signals:
    
public slots:
    
};

class DictionaryDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DictionaryDelegate(QObject *parent = 0);
    void paint (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index);
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
private slots:
    void slotRemoveRow();
    void slotAddRow();
private:
    void drawPixmap(QPainter *painter, const QRect &rct, int type) const;
    /*void setColumnWidth(); */
};

#endif // DICTIONARYMODEL_H
