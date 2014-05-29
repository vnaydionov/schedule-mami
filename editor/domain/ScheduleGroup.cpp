#include "domain/ScheduleGroup.h"
#include <orm/domain_factory.h>
namespace Domain {

ScheduleGroup::Columns ScheduleGroup::c;

namespace {
    Yb::Tables tbls;
    Yb::Relations rels;
    Yb::DomainMetaDataCreator<ScheduleGroup> mdc(tbls, rels);
}

// AUTOGEN_BEGIN(008) {
ScheduleGroup::Columns::Columns()
    : id(_T("ID"), Yb::Value::LONGINT, 0, Yb::Column::PK, Yb::Value(), _T(""), _T(""), _T("id"), _T("id"))
    , schedule_id(_T("SCHEDULE_ID"), Yb::Value::LONGINT, 0, 0, Yb::Value(), _T("T_SCHEDULE"), _T("ID"), _T("schedule-id"), _T("schedule_id"))
    , group_id(_T("GROUP_ID"), Yb::Value::LONGINT, 0, 0, Yb::Value(), _T("T_STUD_GROUP"), _T("ID"), _T("group-id"), _T("group_id"))
{}

void ScheduleGroup::Columns::fill_table(Yb::Table &tbl)
{
    tbl << id << schedule_id << group_id;
}

void ScheduleGroup::create_tables_meta(Yb::Tables &tbls)
{
    Yb::Table::Ptr t(new Yb::Table(_T("T_SCHEDULE_GROUP"), _T("schedule-group"), _T("ScheduleGroup")));
    t->set_seq_name(_T("S_SCHEDULE_GROUP"));
    c.fill_table(*t);
    tbls.push_back(t);
}

void ScheduleGroup::create_relations_meta(Yb::Relations &rels)
{
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("groups");
        attr2[_T("property")] = _T("schedule");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("Schedule"), attr1, _T("ScheduleGroup"), attr2, Yb::Relation::Delete));
        rels.push_back(r);
    }
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr2[_T("property")] = _T("group");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("StudGroup"), attr1, _T("ScheduleGroup"), attr2, Yb::Relation::Delete));
        rels.push_back(r);
    }
}
// } AUTOGEN_END

ScheduleGroup::ScheduleGroup(Yb::DomainObject *owner, const Yb::String &prop_name)
    : Yb::DomainObject(*tbls[0], owner, prop_name)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , group(this, _T("group"))
// } AUTOGEN_END
{}

ScheduleGroup::ScheduleGroup()
    : Yb::DomainObject(*tbls[0])
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , group(this, _T("group"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
}

ScheduleGroup::ScheduleGroup(const ScheduleGroup &other)
    : Yb::DomainObject(other)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , group(this, _T("group"))
// } AUTOGEN_END
{}

ScheduleGroup::ScheduleGroup(Yb::Session &session)
    : Yb::DomainObject(session.schema(), _T("T_SCHEDULE_GROUP"))
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , group(this, _T("group"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
    save(session);
}

ScheduleGroup::ScheduleGroup(Yb::DataObject::Ptr d)
    : Yb::DomainObject(d)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , group(this, _T("group"))
// } AUTOGEN_END
{}

ScheduleGroup::ScheduleGroup(Yb::Session &session, const Yb::Key &key)
    : Yb::DomainObject(session, key)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , group(this, _T("group"))
// } AUTOGEN_END
{}

ScheduleGroup::ScheduleGroup(Yb::Session &session, Yb::LongInt id)
    : Yb::DomainObject(session, _T("T_SCHEDULE_GROUP"), id)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , schedule(this, _T("schedule"))
    , group(this, _T("group"))
// } AUTOGEN_END
{}

ScheduleGroup &ScheduleGroup::operator=(const ScheduleGroup &other)
{
    if (this != &other) {
        *(Yb::DomainObject *)this = (const Yb::DomainObject &)other;
    }
    return *this;
}

ScheduleGroup::ListPtr
ScheduleGroup::find(Yb::Session &session,
        const Yb::Expression &filter, const Yb::Expression &order_by)
{
    ScheduleGroup::ListPtr lst(new ScheduleGroup::List());
    Yb::ObjectList rows;
    session.load_collection(rows, Yb::Expression(_T("T_SCHEDULE_GROUP")), filter, order_by);
    if (rows.size()) {
        Yb::ObjectList::iterator it = rows.begin(), end = rows.end();
        for (; it != end; ++it)
            lst->push_back(ScheduleGroup(*it));
    }
    return lst;
}

struct ScheduleGroupRegistrator
{
    static void register_domain() {
        Yb::theDomainFactory().register_creator(_T("T_SCHEDULE_GROUP"),
            Yb::CreatorPtr(new Yb::DomainCreator<ScheduleGroup>()));
    }
    ScheduleGroupRegistrator() { register_domain(); }
};

static ScheduleGroupRegistrator ScheduleGroup_registrator;

} // end namespace Domain

// vim:ts=4:sts=4:sw=4:et:
