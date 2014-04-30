#ifndef ORM_DOMAIN__PLACE__INCLUDED
#define ORM_DOMAIN__PLACE__INCLUDED

#include <orm/DomainObject.h>
// AUTOGEN_BEGIN(001) {
// } AUTOGEN_END

namespace Domain {

// AUTOGEN_BEGIN(002) {
class Schedule;
typedef Yb::DomainObjHolder<Schedule> ScheduleHolder;
// } AUTOGEN_END

class Place;
typedef Yb::DomainObjHolder<Place> PlaceHolder;

class Place: public Yb::DomainObject
{
public:
    typedef Yb::DomainObjHolder<Place> Holder;
// AUTOGEN_BEGIN(007) {
    struct Columns {
        Yb::Column id, name, short_name;
        Columns();
        void fill_table(Yb::Table &tbl);
    };
    static Columns c;
// } AUTOGEN_END
    static const Yb::String get_table_name() { return _T("T_PLACE"); }
    typedef Yb::DomainResultSet<Place> ResultSet;
    // static method 'find'
    typedef std::vector<Place> List;
    typedef std::auto_ptr<List> ListPtr;
    static ListPtr find(Yb::Session &session,
            const Yb::Expression &filter, const Yb::Expression &order_by = Yb::Expression());
    // constructors
    Place(Yb::DomainObject *owner, const Yb::String &prop_name);
    Place();
    Place(const Place &other);
    explicit Place(Yb::Session &session);
    explicit Place(Yb::DataObject::Ptr d);
    Place(Yb::Session &session, const Yb::Key &key);
    Place(Yb::Session &session, Yb::LongInt id);
    Place &operator=(const Place &other);
    static void create_tables_meta(Yb::Tables &tbls);
    static void create_relations_meta(Yb::Relations &rels);
// AUTOGEN_BEGIN(003) {
    // properties
    Yb::Property<Yb::LongInt, 0> id;
    Yb::Property<Yb::String, 1> name;
    Yb::Property<Yb::String, 2> short_name;
    // relation properties
    Yb::ManagedList<Schedule> classes;
// } AUTOGEN_END
};

} // namespace Domain

// vim:ts=4:sts=4:sw=4:et:
#endif
