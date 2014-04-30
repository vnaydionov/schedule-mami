#include "sharedobj.h"

SharedObj::SharedObj()
{
    Yb::init_schema();
    std::auto_ptr<Yb::SqlConnection> conn(new Yb::SqlConnection("mysql+qodbc3://schedule"));
    engine_.reset(new Yb::Engine(Yb::Engine::READ_WRITE, conn));
    engine_->set_echo(true);
}

void
SharedObj::setCategories(QStringList categories)
{
    categories_ = categories;
}

void
SharedObj::setRows(QVector<Row> rows)
{
    rows_ = rows;
}
