#include "domain/ScheduleDate.h"
#include <orm/domain_factory.h>
namespace Domain {

ScheduleDate::Columns ScheduleDate::c;

namespace {
    Yb::Tables tbls;
    Yb::Relations rels;
    Yb::DomainMetaDataCreator<ScheduleDate> mdc(tbls, rels);
}

// AUTOGEN_BEGIN(008) {
ScheduleDate::Columns::Columns()
    : id(_T("ID"), Yb::Value::LONGINT, 0, Yb::Column::PK, Yb::Value(), _T(""), _T(""), _T("id"), _T("id"))
    , item_type(_T("ITEM_TYPE"), Yb::Value::STRING, 10, 0, Yb::Value(), _T(""), _T(""), _T("item-type"), _T("item_type"))
    , schedule_id(_T("SCHEDULE_ID"), Yb::Value::LONGINT, 0, 0, Yb::Value(), _T("T_SCHEDULE"), _T("ID"), _T("schedule-id"), _T("schedule_id"))
    , dt(_T("DT"), Yb::Value::DATETIME, 0, 0, Yb::Value(), _T(""), _T(""), _T("dt"), _T("dt"))
{}

void ScheduleDate::Columns::fill_table(Yb::Table &tbl)
{
    tbl << id << item_type << schedule_id << dt;
}

void ScheduleDate::create_tables_meta(Yb::Tables &tbls)
{
    Yb::Table::Ptr t(new Yb::Table(_T("T_SCHEDULE_DATE"), _T("schedule-date"), _T("ScheduleDate")));
    t->set_seq_name(_T("S_SCHEDULE_DATE"));
    c.fill_table(*t);
    tbls.push_back(t);
}

void ScheduleDate::create_relations_meta(Yb::Relations &rels)
{
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("dates");
        attr2[_T("property")] = _T("schedule");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("Schedule"), attr1, _T("ScheduleDate"), attr2, Yb::Relation::Delete));
        rels.push_back(r);
    }
}
// } AUTOGEN_END

ScheduleDate::ScheduleDate(Yb::DomainObject *owner, const Yb::String &prop_name)
    : Yb::DomainObject(*tbls[0], owner, prop_name)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , item_type(this)
    , dt(this)
    , schedule(this, _T("schedule"))
// } AUTOGEN_END
{}

ScheduleDate::ScheduleDate()
    : Yb::DomainObject(*tbls[0])
// AUTOGEN_BEGIN(005) {
    , id(this)
    , item_type(this)
    , dt(this)
    , schedule(this, _T("schedule"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
}

ScheduleDate::ScheduleDate(const ScheduleDate &other)
    : Yb::DomainObject(other)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , item_type(this)
    , dt(this)
    , schedule(this, _T("schedule"))
// } AUTOGEN_END
{}

ScheduleDate::ScheduleDate(Yb::Session &session)
    : Yb::DomainObject(session.schema(), _T("T_SCHEDULE_DATE"))
// AUTOGEN_BEGIN(005) {
    , id(this)
    , item_type(this)
    , dt(this)
    , schedule(this, _T("schedule"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
    save(session);
}

ScheduleDate::ScheduleDate(Yb::DataObject::Ptr d)
    : Yb::DomainObject(d)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , item_type(this)
    , dt(this)
    , schedule(this, _T("schedule"))
// } AUTOGEN_END
{}

ScheduleDate::ScheduleDate(Yb::Session &session, const Yb::Key &key)
    : Yb::DomainObject(session, key)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , item_type(this)
    , dt(this)
    , schedule(this, _T("schedule"))
// } AUTOGEN_END
{}

ScheduleDate::ScheduleDate(Yb::Session &session, Yb::LongInt id)
    : Yb::DomainObject(session, _T("T_SCHEDULE_DATE"), id)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , item_type(this)
    , dt(this)
    , schedule(this, _T("schedule"))
// } AUTOGEN_END
{}

ScheduleDate &ScheduleDate::operator=(const ScheduleDate &other)
{
    if (this != &other) {
        *(Yb::DomainObject *)this = (const Yb::DomainObject &)other;
    }
    return *this;
}

ScheduleDate::ListPtr
ScheduleDate::find(Yb::Session &session,
        const Yb::Expression &filter, const Yb::Expression &order_by)
{
    ScheduleDate::ListPtr lst(new ScheduleDate::List());
    Yb::ObjectList rows;
    session.load_collection(rows, Yb::Expression(_T("T_SCHEDULE_DATE")), filter, order_by);
    if (rows.size()) {
        Yb::ObjectList::iterator it = rows.begin(), end = rows.end();
        for (; it != end; ++it)
            lst->push_back(ScheduleDate(*it));
    }
    return lst;
}

struct ScheduleDateRegistrator
{
    static void register_domain() {
        Yb::theDomainFactory().register_creator(_T("T_SCHEDULE_DATE"),
            Yb::CreatorPtr(new Yb::DomainCreator<ScheduleDate>()));
    }
    ScheduleDateRegistrator() { register_domain(); }
};

static ScheduleDateRegistrator ScheduleDate_registrator;

} // end namespace Domain

// vim:ts=4:sts=4:sw=4:et:
