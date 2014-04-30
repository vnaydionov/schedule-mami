#ifndef XMLCONFIGMODEL_H
#define XMLCONFIGMODEL_H

#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QStringList>
#include <QVector>
#include "App.h"

struct RowInfo
{
    QString m_eng_name;
    QString m_rus_name;
    bool m_visible;
    int m_width;
    RowInfo() :
        m_eng_name(""), m_rus_name(""),
        m_visible(false), m_width(0) {}
};

class XmlConfigModel : public QAbstractItemModel
{
public:
    explicit XmlConfigModel(QString tbl_name, QObject *parent = 0);
    ~XmlConfigModel();
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QString m_tbl_name;
    QVector<RowInfo> m_columns_info;
    QStringList m_feature_name;
};

class XmlConfigDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit XmlConfigDelegate(QObject *parent = 0);
    QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

#endif // XMLCONFIGMODEL_H
