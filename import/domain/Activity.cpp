#include "domain/Activity.h"
#include <orm/domain_factory.h>
namespace Domain {

Activity::Columns Activity::c;

namespace {
    Yb::Tables tbls;
    Yb::Relations rels;
    Yb::DomainMetaDataCreator<Activity> mdc(tbls, rels);
}

// AUTOGEN_BEGIN(008) {
Activity::Columns::Columns()
    : id(_T("ID"), Yb::Value::LONGINT, 0, Yb::Column::PK, Yb::Value(), _T(""), _T(""), _T("id"), _T("id"))
    , name(_T("NAME"), Yb::Value::STRING, 100, 0, Yb::Value(), _T(""), _T(""), _T("name"), _T("name"))
    , short_name(_T("SHORT_NAME"), Yb::Value::STRING, 20, 0, Yb::Value(), _T(""), _T(""), _T("short-name"), _T("short_name"))
{}

void Activity::Columns::fill_table(Yb::Table &tbl)
{
    tbl << id << name << short_name;
}

void Activity::create_tables_meta(Yb::Tables &tbls)
{
    Yb::Table::Ptr t(new Yb::Table(_T("T_ACTIVITY"), _T("activity"), _T("Activity")));
    t->set_seq_name(_T("S_ACTIVITY"));
    c.fill_table(*t);
    tbls.push_back(t);
}

void Activity::create_relations_meta(Yb::Relations &rels)
{
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr2[_T("property")] = _T("activity");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("Activity"), attr1, _T("ScheduleActivity"), attr2, Yb::Relation::Delete));
        rels.push_back(r);
    }
}
// } AUTOGEN_END

Activity::Activity(Yb::DomainObject *owner, const Yb::String &prop_name)
    : Yb::DomainObject(*tbls[0], owner, prop_name)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
// } AUTOGEN_END
{}

Activity::Activity()
    : Yb::DomainObject(*tbls[0])
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
}

Activity::Activity(const Activity &other)
    : Yb::DomainObject(other)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
// } AUTOGEN_END
{}

Activity::Activity(Yb::Session &session)
    : Yb::DomainObject(session.schema(), _T("T_ACTIVITY"))
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
    save(session);
}

Activity::Activity(Yb::DataObject::Ptr d)
    : Yb::DomainObject(d)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
// } AUTOGEN_END
{}

Activity::Activity(Yb::Session &session, const Yb::Key &key)
    : Yb::DomainObject(session, key)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
// } AUTOGEN_END
{}

Activity::Activity(Yb::Session &session, Yb::LongInt id)
    : Yb::DomainObject(session, _T("T_ACTIVITY"), id)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
// } AUTOGEN_END
{}

Activity &Activity::operator=(const Activity &other)
{
    if (this != &other) {
        *(Yb::DomainObject *)this = (const Yb::DomainObject &)other;
    }
    return *this;
}

Activity::ListPtr
Activity::find(Yb::Session &session,
        const Yb::Expression &filter, const Yb::Expression &order_by)
{
    Activity::ListPtr lst(new Activity::List());
    Yb::ObjectList rows;
    session.load_collection(rows, Yb::Expression(_T("T_ACTIVITY")), filter, order_by);
    if (rows.size()) {
        Yb::ObjectList::iterator it = rows.begin(), end = rows.end();
        for (; it != end; ++it)
            lst->push_back(Activity(*it));
    }
    return lst;
}

struct ActivityRegistrator
{
    static void register_domain() {
        Yb::theDomainFactory().register_creator(_T("T_ACTIVITY"),
            Yb::CreatorPtr(new Yb::DomainCreator<Activity>()));
    }
    ActivityRegistrator() { register_domain(); }
};

static ActivityRegistrator Activity_registrator;

} // end namespace Domain

// vim:ts=4:sts=4:sw=4:et:
