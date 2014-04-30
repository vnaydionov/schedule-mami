#ifndef SHAREDOBJ_H
#define SHAREDOBJ_H

#include <orm/MetaData.h>
#include <orm/Engine.h>
#include <QStringList>
#include <QScopedPointer>
#include "documentparser.h"

typedef std::auto_ptr<Yb::Engine> EnginePtr;

class SharedObj
{
private:
    EnginePtr engine_;
    QStringList categories_;
    QVector<Row> rows_;
public:
    SharedObj();
    void setCategories(QStringList categories);
    void setRows(QVector<Row> rows);
    inline Yb::Engine* getEngine()
    {
        return engine_.get();
    }

    inline QStringList getCategories() const
    {
        return categories_;
    }

    inline QVector<Row> getRows() const
    {
        return rows_;
    }
};

typedef Yb::SingletonHolder<SharedObj> SharedObjSingleton;


#endif // SHAREDOBJ_H
