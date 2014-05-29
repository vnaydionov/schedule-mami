#include "domain/Schedule.h"
#include <orm/domain_factory.h>
namespace Domain {

Schedule::Columns Schedule::c;

namespace {
    Yb::Tables tbls;
    Yb::Relations rels;
    Yb::DomainMetaDataCreator<Schedule> mdc(tbls, rels);
}

// AUTOGEN_BEGIN(008) {
Schedule::Columns::Columns()
    : id(_T("ID"), Yb::Value::LONGINT, 0, Yb::Column::PK, Yb::Value(), _T(""), _T(""), _T("id"), _T("id"))
    , update_dt(_T("UPDATE_DT"), Yb::Value::DATETIME, 0, 0, Yb::Value(_T("sysdate")), _T(""), _T(""), _T("update-dt"), _T("update_dt"))
    , teacher_id(_T("TEACHER_ID"), Yb::Value::LONGINT, 0, 0, Yb::Value(), _T("T_USER"), _T("ID"), _T("teacher-id"), _T("teacher_id"))
    , receiver_id(_T("RECEIVER_ID"), Yb::Value::LONGINT, 0, 0, Yb::Value(), _T("T_USER"), _T("ID"), _T("receiver-id"), _T("receiver_id"))
    , subject_id(_T("SUBJECT_ID"), Yb::Value::LONGINT, 0, 0, Yb::Value(), _T("T_SUBJECT"), _T("ID"), _T("subject-id"), _T("subject_id"))
    , place_id(_T("PLACE_ID"), Yb::Value::LONGINT, 0, 0, Yb::Value(), _T("T_PLACE"), _T("ID"), _T("place-id"), _T("place_id"))
    , start_dt(_T("START_DT"), Yb::Value::DATETIME, 0, Yb::Column::NULLABLE, Yb::Value(), _T(""), _T(""), _T("start-dt"), _T("start_dt"))
    , end_dt(_T("END_DT"), Yb::Value::DATETIME, 0, Yb::Column::NULLABLE, Yb::Value(), _T(""), _T(""), _T("end-dt"), _T("end_dt"))
    , dow(_T("DOW"), Yb::Value::STRING, 15, Yb::Column::NULLABLE, Yb::Value(), _T(""), _T(""), _T("dow"), _T("dow"))
    , repeat_type(_T("REPEAT_TYPE"), Yb::Value::STRING, 10, Yb::Column::NULLABLE, Yb::Value(), _T(""), _T(""), _T("repeat-type"), _T("repeat_type"))
    , start_time(_T("START_TIME"), Yb::Value::INTEGER, 0, Yb::Column::NULLABLE, Yb::Value(), _T(""), _T(""), _T("start-time"), _T("start_time"))
    , end_time(_T("END_TIME"), Yb::Value::INTEGER, 0, Yb::Column::NULLABLE, Yb::Value(), _T(""), _T(""), _T("end-time"), _T("end_time"))
    , sent(_T("SENT"), Yb::Value::INTEGER, 0, 0, Yb::Value(_T("0")), _T(""), _T(""), _T("sent"), _T("sent"))
    , checked(_T("CHECKED"), Yb::Value::INTEGER, 0, 0, Yb::Value(_T("0")), _T(""), _T(""), _T("checked"), _T("checked"))
{}

void Schedule::Columns::fill_table(Yb::Table &tbl)
{
    tbl << id << update_dt << teacher_id << receiver_id << subject_id << place_id << start_dt << end_dt << dow << repeat_type << start_time << end_time << sent << checked;
}

void Schedule::create_tables_meta(Yb::Tables &tbls)
{
    Yb::Table::Ptr t(new Yb::Table(_T("T_SCHEDULE"), _T("schedule"), _T("Schedule")));
    t->set_seq_name(_T("S_SCHEDULE"));
    c.fill_table(*t);
    tbls.push_back(t);
}

void Schedule::create_relations_meta(Yb::Relations &rels)
{
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("classes");
        attr2[_T("key")] = _T("TEACHER_ID");
        attr2[_T("property")] = _T("teacher");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("User"), attr1, _T("Schedule"), attr2, Yb::Relation::Restrict));
        rels.push_back(r);
    }
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("subscriptions");
        attr2[_T("key")] = _T("RECEIVER_ID");
        attr2[_T("property")] = _T("receiver");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("User"), attr1, _T("Schedule"), attr2, Yb::Relation::Restrict));
        rels.push_back(r);
    }
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("classes");
        attr2[_T("property")] = _T("place");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("Place"), attr1, _T("Schedule"), attr2, Yb::Relation::Restrict));
        rels.push_back(r);
    }
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("classes");
        attr2[_T("property")] = _T("subject");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("Subject"), attr1, _T("Schedule"), attr2, Yb::Relation::Restrict));
        rels.push_back(r);
    }
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("dates");
        attr2[_T("property")] = _T("schedule");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("Schedule"), attr1, _T("ScheduleDate"), attr2, Yb::Relation::Delete));
        rels.push_back(r);
    }
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("activities");
        attr2[_T("property")] = _T("schedule");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("Schedule"), attr1, _T("ScheduleActivity"), attr2, Yb::Relation::Delete));
        rels.push_back(r);
    }
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("groups");
        attr2[_T("property")] = _T("schedule");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("Schedule"), attr1, _T("ScheduleGroup"), attr2, Yb::Relation::Delete));
        rels.push_back(r);
    }
}
// } AUTOGEN_END

Schedule::Schedule(Yb::DomainObject *owner, const Yb::String &prop_name)
    : Yb::DomainObject(*tbls[0], owner, prop_name)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , update_dt(this)
    , start_dt(this)
    , end_dt(this)
    , dow(this)
    , repeat_type(this)
    , start_time(this)
    , end_time(this)
    , sent(this)
    , checked(this)
    , teacher(this, _T("teacher"))
    , receiver(this, _T("receiver"))
    , place(this, _T("place"))
    , subject(this, _T("subject"))
    , dates(this, _T("dates"))
    , activities(this, _T("activities"))
    , groups(this, _T("groups"))
// } AUTOGEN_END
{}

Schedule::Schedule()
    : Yb::DomainObject(*tbls[0])
// AUTOGEN_BEGIN(005) {
    , id(this)
    , update_dt(this)
    , start_dt(this)
    , end_dt(this)
    , dow(this)
    , repeat_type(this)
    , start_time(this)
    , end_time(this)
    , sent(this)
    , checked(this)
    , teacher(this, _T("teacher"))
    , receiver(this, _T("receiver"))
    , place(this, _T("place"))
    , subject(this, _T("subject"))
    , dates(this, _T("dates"))
    , activities(this, _T("activities"))
    , groups(this, _T("groups"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
    set(1, Yb::Value(Yb::now()));
    set(12, Yb::Value((int)0));
    set(13, Yb::Value((int)0));
// } AUTOGEN_END
}

Schedule::Schedule(const Schedule &other)
    : Yb::DomainObject(other)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , update_dt(this)
    , start_dt(this)
    , end_dt(this)
    , dow(this)
    , repeat_type(this)
    , start_time(this)
    , end_time(this)
    , sent(this)
    , checked(this)
    , teacher(this, _T("teacher"))
    , receiver(this, _T("receiver"))
    , place(this, _T("place"))
    , subject(this, _T("subject"))
    , dates(this, _T("dates"))
    , activities(this, _T("activities"))
    , groups(this, _T("groups"))
// } AUTOGEN_END
{}

Schedule::Schedule(Yb::Session &session)
    : Yb::DomainObject(session.schema(), _T("T_SCHEDULE"))
// AUTOGEN_BEGIN(005) {
    , id(this)
    , update_dt(this)
    , start_dt(this)
    , end_dt(this)
    , dow(this)
    , repeat_type(this)
    , start_time(this)
    , end_time(this)
    , sent(this)
    , checked(this)
    , teacher(this, _T("teacher"))
    , receiver(this, _T("receiver"))
    , place(this, _T("place"))
    , subject(this, _T("subject"))
    , dates(this, _T("dates"))
    , activities(this, _T("activities"))
    , groups(this, _T("groups"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
    set(1, Yb::Value(Yb::now()));
    set(12, Yb::Value((int)0));
    set(13, Yb::Value((int)0));
// } AUTOGEN_END
    save(session);
}

Schedule::Schedule(Yb::DataObject::Ptr d)
    : Yb::DomainObject(d)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , update_dt(this)
    , start_dt(this)
    , end_dt(this)
    , dow(this)
    , repeat_type(this)
    , start_time(this)
    , end_time(this)
    , sent(this)
    , checked(this)
    , teacher(this, _T("teacher"))
    , receiver(this, _T("receiver"))
    , place(this, _T("place"))
    , subject(this, _T("subject"))
    , dates(this, _T("dates"))
    , activities(this, _T("activities"))
    , groups(this, _T("groups"))
// } AUTOGEN_END
{}

Schedule::Schedule(Yb::Session &session, const Yb::Key &key)
    : Yb::DomainObject(session, key)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , update_dt(this)
    , start_dt(this)
    , end_dt(this)
    , dow(this)
    , repeat_type(this)
    , start_time(this)
    , end_time(this)
    , sent(this)
    , checked(this)
    , teacher(this, _T("teacher"))
    , receiver(this, _T("receiver"))
    , place(this, _T("place"))
    , subject(this, _T("subject"))
    , dates(this, _T("dates"))
    , activities(this, _T("activities"))
    , groups(this, _T("groups"))
// } AUTOGEN_END
{}

Schedule::Schedule(Yb::Session &session, Yb::LongInt id)
    : Yb::DomainObject(session, _T("T_SCHEDULE"), id)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , update_dt(this)
    , start_dt(this)
    , end_dt(this)
    , dow(this)
    , repeat_type(this)
    , start_time(this)
    , end_time(this)
    , sent(this)
    , checked(this)
    , teacher(this, _T("teacher"))
    , receiver(this, _T("receiver"))
    , place(this, _T("place"))
    , subject(this, _T("subject"))
    , dates(this, _T("dates"))
    , activities(this, _T("activities"))
    , groups(this, _T("groups"))
// } AUTOGEN_END
{}

Schedule &Schedule::operator=(const Schedule &other)
{
    if (this != &other) {
        *(Yb::DomainObject *)this = (const Yb::DomainObject &)other;
    }
    return *this;
}

Schedule::ListPtr
Schedule::find(Yb::Session &session,
        const Yb::Expression &filter, const Yb::Expression &order_by)
{
    Schedule::ListPtr lst(new Schedule::List());
    Yb::ObjectList rows;
    session.load_collection(rows, Yb::Expression(_T("T_SCHEDULE")), filter, order_by);
    if (rows.size()) {
        Yb::ObjectList::iterator it = rows.begin(), end = rows.end();
        for (; it != end; ++it)
            lst->push_back(Schedule(*it));
    }
    return lst;
}

struct ScheduleRegistrator
{
    static void register_domain() {
        Yb::theDomainFactory().register_creator(_T("T_SCHEDULE"),
            Yb::CreatorPtr(new Yb::DomainCreator<Schedule>()));
    }
    ScheduleRegistrator() { register_domain(); }
};

static ScheduleRegistrator Schedule_registrator;

} // end namespace Domain

// vim:ts=4:sts=4:sw=4:et:
