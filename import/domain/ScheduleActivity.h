#ifndef ORM_DOMAIN__SCHEDULEACTIVITY__INCLUDED
#define ORM_DOMAIN__SCHEDULEACTIVITY__INCLUDED

#include <orm/domain_object.h>
// AUTOGEN_BEGIN(001) {
#include "domain/Schedule.h"
#include "domain/Activity.h"
// } AUTOGEN_END

namespace Domain {

// AUTOGEN_BEGIN(002) {
// } AUTOGEN_END

class ScheduleActivity;
typedef Yb::DomainObjHolder<ScheduleActivity> ScheduleActivityHolder;

class ScheduleActivity: public Yb::DomainObject
{
public:
    typedef Yb::DomainObjHolder<ScheduleActivity> Holder;
// AUTOGEN_BEGIN(007) {
    struct Columns {
        Yb::Column id, schedule_id, activity_id;
        Columns();
        void fill_table(Yb::Table &tbl);
    };
    static Columns c;
// } AUTOGEN_END
    static const Yb::String get_table_name() { return _T("T_SCHEDULE_ACTIVITY"); }
    typedef Yb::DomainResultSet<ScheduleActivity> ResultSet;
    // static method 'find'
    typedef std::vector<ScheduleActivity> List;
    typedef std::auto_ptr<List> ListPtr;
    static ListPtr find(Yb::Session &session,
            const Yb::Expression &filter, const Yb::Expression &order_by = Yb::Expression());
    // constructors
    ScheduleActivity(Yb::DomainObject *owner, const Yb::String &prop_name);
    ScheduleActivity();
    ScheduleActivity(const ScheduleActivity &other);
    explicit ScheduleActivity(Yb::Session &session);
    explicit ScheduleActivity(Yb::DataObject::Ptr d);
    ScheduleActivity(Yb::Session &session, const Yb::Key &key);
    ScheduleActivity(Yb::Session &session, Yb::LongInt id);
    ScheduleActivity &operator=(const ScheduleActivity &other);
    static void create_tables_meta(Yb::Tables &tbls);
    static void create_relations_meta(Yb::Relations &rels);
// AUTOGEN_BEGIN(003) {
    // properties
    Yb::Property<Yb::LongInt, 0> id;
    // relation properties
    Schedule::Holder schedule;
    Activity::Holder activity;
// } AUTOGEN_END
};

} // namespace Domain

// vim:ts=4:sts=4:sw=4:et:
#endif
