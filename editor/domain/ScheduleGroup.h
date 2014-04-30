#ifndef ORM_DOMAIN__SCHEDULEGROUP__INCLUDED
#define ORM_DOMAIN__SCHEDULEGROUP__INCLUDED

#include <orm/DomainObject.h>
// AUTOGEN_BEGIN(001) {
#include "domain/Schedule.h"
#include "domain/StudGroup.h"
// } AUTOGEN_END

namespace Domain {

// AUTOGEN_BEGIN(002) {
// } AUTOGEN_END

class ScheduleGroup;
typedef Yb::DomainObjHolder<ScheduleGroup> ScheduleGroupHolder;

class ScheduleGroup: public Yb::DomainObject
{
public:
    typedef Yb::DomainObjHolder<ScheduleGroup> Holder;
// AUTOGEN_BEGIN(007) {
    struct Columns {
        Yb::Column id, schedule_id, group_id;
        Columns();
        void fill_table(Yb::Table &tbl);
    };
    static Columns c;
// } AUTOGEN_END
    static const Yb::String get_table_name() { return _T("T_SCHEDULE_GROUP"); }
    typedef Yb::DomainResultSet<ScheduleGroup> ResultSet;
    // static method 'find'
    typedef std::vector<ScheduleGroup> List;
    typedef std::auto_ptr<List> ListPtr;
    static ListPtr find(Yb::Session &session,
            const Yb::Expression &filter, const Yb::Expression &order_by = Yb::Expression());
    // constructors
    ScheduleGroup(Yb::DomainObject *owner, const Yb::String &prop_name);
    ScheduleGroup();
    ScheduleGroup(const ScheduleGroup &other);
    explicit ScheduleGroup(Yb::Session &session);
    explicit ScheduleGroup(Yb::DataObject::Ptr d);
    ScheduleGroup(Yb::Session &session, const Yb::Key &key);
    ScheduleGroup(Yb::Session &session, Yb::LongInt id);
    ScheduleGroup &operator=(const ScheduleGroup &other);
    static void create_tables_meta(Yb::Tables &tbls);
    static void create_relations_meta(Yb::Relations &rels);
// AUTOGEN_BEGIN(003) {
    // properties
    Yb::Property<Yb::LongInt, 0> id;
    // relation properties
    Schedule::Holder schedule;
    StudGroup::Holder group;
// } AUTOGEN_END
};

} // namespace Domain

// vim:ts=4:sts=4:sw=4:et:
#endif
