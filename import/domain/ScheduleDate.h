#ifndef ORM_DOMAIN__SCHEDULEDATE__INCLUDED
#define ORM_DOMAIN__SCHEDULEDATE__INCLUDED

#include <orm/DomainObject.h>
// AUTOGEN_BEGIN(001) {
#include "domain/Schedule.h"
// } AUTOGEN_END

namespace Domain {

// AUTOGEN_BEGIN(002) {
// } AUTOGEN_END

class ScheduleDate;
typedef Yb::DomainObjHolder<ScheduleDate> ScheduleDateHolder;

class ScheduleDate: public Yb::DomainObject
{
public:
    typedef Yb::DomainObjHolder<ScheduleDate> Holder;
// AUTOGEN_BEGIN(007) {
    struct Columns {
        Yb::Column id, item_type, schedule_id, dt;
        Columns();
        void fill_table(Yb::Table &tbl);
    };
    static Columns c;
// } AUTOGEN_END
    static const Yb::String get_table_name() { return _T("T_SCHEDULE_DATE"); }
    typedef Yb::DomainResultSet<ScheduleDate> ResultSet;
    // static method 'find'
    typedef std::vector<ScheduleDate> List;
    typedef std::auto_ptr<List> ListPtr;
    static ListPtr find(Yb::Session &session,
            const Yb::Expression &filter, const Yb::Expression &order_by = Yb::Expression());
    // constructors
    ScheduleDate(Yb::DomainObject *owner, const Yb::String &prop_name);
    ScheduleDate();
    ScheduleDate(const ScheduleDate &other);
    explicit ScheduleDate(Yb::Session &session);
    explicit ScheduleDate(Yb::DataObject::Ptr d);
    ScheduleDate(Yb::Session &session, const Yb::Key &key);
    ScheduleDate(Yb::Session &session, Yb::LongInt id);
    ScheduleDate &operator=(const ScheduleDate &other);
    static void create_tables_meta(Yb::Tables &tbls);
    static void create_relations_meta(Yb::Relations &rels);
// AUTOGEN_BEGIN(003) {
    // properties
    Yb::Property<Yb::LongInt, 0> id;
    Yb::Property<Yb::String, 1> item_type;
    Yb::Property<Yb::DateTime, 3> dt;
    // relation properties
    Schedule::Holder schedule;
// } AUTOGEN_END
};

} // namespace Domain

// vim:ts=4:sts=4:sw=4:et:
#endif
