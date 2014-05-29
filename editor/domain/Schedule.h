#ifndef ORM_DOMAIN__SCHEDULE__INCLUDED
#define ORM_DOMAIN__SCHEDULE__INCLUDED

#include <orm/domain_object.h>
// AUTOGEN_BEGIN(001) {
#include "domain/User.h"
#include "domain/User.h"
#include "domain/Subject.h"
#include "domain/Place.h"
// } AUTOGEN_END

namespace Domain {

// AUTOGEN_BEGIN(002) {
class ScheduleActivity;
typedef Yb::DomainObjHolder<ScheduleActivity> ScheduleActivityHolder;
class ScheduleDate;
typedef Yb::DomainObjHolder<ScheduleDate> ScheduleDateHolder;
class ScheduleGroup;
typedef Yb::DomainObjHolder<ScheduleGroup> ScheduleGroupHolder;
// } AUTOGEN_END

class Schedule;
typedef Yb::DomainObjHolder<Schedule> ScheduleHolder;

class Schedule: public Yb::DomainObject
{
public:
    typedef Yb::DomainObjHolder<Schedule> Holder;
// AUTOGEN_BEGIN(007) {
    struct Columns {
        Yb::Column id, update_dt, teacher_id, receiver_id, subject_id, place_id, start_dt, end_dt, dow, repeat_type, start_time, end_time, sent, checked;
        Columns();
        void fill_table(Yb::Table &tbl);
    };
    static Columns c;
// } AUTOGEN_END
    static const Yb::String get_table_name() { return _T("T_SCHEDULE"); }
    typedef Yb::DomainResultSet<Schedule> ResultSet;
    // static method 'find'
    typedef std::vector<Schedule> List;
    typedef std::auto_ptr<List> ListPtr;
    static ListPtr find(Yb::Session &session,
            const Yb::Expression &filter, const Yb::Expression &order_by = Yb::Expression());
    // constructors
    Schedule(Yb::DomainObject *owner, const Yb::String &prop_name);
    Schedule();
    Schedule(const Schedule &other);
    explicit Schedule(Yb::Session &session);
    explicit Schedule(Yb::DataObject::Ptr d);
    Schedule(Yb::Session &session, const Yb::Key &key);
    Schedule(Yb::Session &session, Yb::LongInt id);
    Schedule &operator=(const Schedule &other);
    static void create_tables_meta(Yb::Tables &tbls);
    static void create_relations_meta(Yb::Relations &rels);
// AUTOGEN_BEGIN(003) {
    // properties
    Yb::Property<Yb::LongInt, 0> id;
    Yb::Property<Yb::DateTime, 1> update_dt;
    Yb::Property<Yb::DateTime, 6> start_dt;
    Yb::Property<Yb::DateTime, 7> end_dt;
    Yb::Property<Yb::String, 8> dow;
    Yb::Property<Yb::String, 9> repeat_type;
    Yb::Property<int, 10> start_time;
    Yb::Property<int, 11> end_time;
    Yb::Property<int, 12> sent;
    Yb::Property<int, 13> checked;
    // relation properties
    User::Holder teacher;
    User::Holder receiver;
    Place::Holder place;
    Subject::Holder subject;
    Yb::ManagedList<ScheduleDate> dates;
    Yb::ManagedList<ScheduleActivity> activities;
    Yb::ManagedList<ScheduleGroup> groups;
// } AUTOGEN_END
};

} // namespace Domain

// vim:ts=4:sts=4:sw=4:et:
#endif
