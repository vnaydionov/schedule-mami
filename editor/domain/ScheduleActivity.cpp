#include "domain/ScheduleActivity.h"
#include <orm/domain_factory.h>
namespace Domain {

ScheduleActivity::Columns ScheduleActivity::c;

namespace {
    Yb::Tables tbls;
    Yb::Relations rels;
    Yb::DomainMetaDataCreator<ScheduleActivity> mdc(tbls, rels);
}

// AUTOGEN_BEGIN(008) {
ScheduleActivity::Columns::Columns()
    : id(_T("ID"), Yb::Value::LONGINT, 0, Yb::Column::PK, Yb::Value(), _T(""), _T(""), _T("id"), _T("id"))
    , schedule_id(_T("SCHEDULE_ID"), Yb::Value::LONGINT, 0, 0, Yb::Value(), _T("T_SCHEDULE"), _T("ID"), _T("schedule-id"), _T("schedule_id"))
    , activity_id(_T("ACTIVITY_ID"), Yb::Value::LONGINT, 0, 0, Yb::Value(), _T("T_ACTIVITY"), _T("ID"), _T("activity-id"), _T("activity_id"))
{}

void ScheduleActivity::Columns::fill_table(Yb::Table &tbl)
{
    tbl << id << schedule_id << activity_id;
}

void ScheduleActivity::create_tables_meta(Yb::Tables &tbls)
{
    Yb::Table::Ptr t(new Yb::Table(_T("T_SCHEDULE_ACTIVITY"), _T("schedule-activity"), _T("ScheduleActivity")));
    t->set_seq_name(_T("S_SCHEDULE_ACTIVITY"));
    c.fill_table(*t);
    tbls.push_back(t);
}

void ScheduleActivity::create_relations_meta(Yb::Relations &rels)
{
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("activities");
        attr2[_T("property")] = _T("schedule");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("Schedule"), attr1, _T("ScheduleActivity"), attr2, Yb::Relation::Delete));
        rels.push_back(r);
    }
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr2[_T("property")] = _T("activity");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("Activity"), attr1, _T("ScheduleActivity"), attr2, Yb::Relation::Delete));
        rels.push_back(r);
    }
}
// } AUTOGEN_END

ScheduleActivity::ScheduleActivity(Yb::DomainObject *owner, const Yb::String &prop_name)
    : Yb::DomainObject(*tbls[0], owner, prop_name)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , activity(this, _T("activity"))
// } AUTOGEN_END
{}

ScheduleActivity::ScheduleActivity()
    : Yb::DomainObject(*tbls[0])
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , activity(this, _T("activity"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
}

ScheduleActivity::ScheduleActivity(const ScheduleActivity &other)
    : Yb::DomainObject(other)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , activity(this, _T("activity"))
// } AUTOGEN_END
{}

ScheduleActivity::ScheduleActivity(Yb::Session &session)
    : Yb::DomainObject(session.schema(), _T("T_SCHEDULE_ACTIVITY"))
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , activity(this, _T("activity"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
    save(session);
}

ScheduleActivity::ScheduleActivity(Yb::DataObject::Ptr d)
    : Yb::DomainObject(d)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , activity(this, _T("activity"))
// } AUTOGEN_END
{}

ScheduleActivity::ScheduleActivity(Yb::Session &session, const Yb::Key &key)
    : Yb::DomainObject(session, key)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , activity(this, _T("activity"))
// } AUTOGEN_END
{}

ScheduleActivity::ScheduleActivity(Yb::Session &session, Yb::LongInt id)
    : Yb::DomainObject(session, _T("T_SCHEDULE_ACTIVITY"), id)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , activity(this, _T("activity"))
// } AUTOGEN_END
{}

ScheduleActivity &ScheduleActivity::operator=(const ScheduleActivity &other)
{
    if (this != &other) {
        *(Yb::DomainObject *)this = (const Yb::DomainObject &)other;
    }
    return *this;
}

ScheduleActivity::ListPtr
ScheduleActivity::find(Yb::Session &session,
        const Yb::Expression &filter, const Yb::Expression &order_by)
{
    ScheduleActivity::ListPtr lst(new ScheduleActivity::List());
    Yb::ObjectList rows;
    session.load_collection(rows, Yb::Expression(_T("T_SCHEDULE_ACTIVITY")), filter, order_by);
    if (rows.size()) {
        Yb::ObjectList::iterator it = rows.begin(), end = rows.end();
        for (; it != end; ++it)
            lst->push_back(ScheduleActivity(*it));
    }
    return lst;
}

struct ScheduleActivityRegistrator
{
    static void register_domain() {
        Yb::theDomainFactory().register_creator(_T("T_SCHEDULE_ACTIVITY"),
            Yb::CreatorPtr(new Yb::DomainCreator<ScheduleActivity>()));
    }
    ScheduleActivityRegistrator() { register_domain(); }
};

static ScheduleActivityRegistrator ScheduleActivity_registrator;

} // end namespace Domain

// vim:ts=4:sts=4:sw=4:et:
