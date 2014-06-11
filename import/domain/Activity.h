#ifndef ORM_DOMAIN__ACTIVITY__INCLUDED
#define ORM_DOMAIN__ACTIVITY__INCLUDED

#include <orm/DomainObject.h>
// AUTOGEN_BEGIN(001) {
// } AUTOGEN_END

namespace Domain {

// AUTOGEN_BEGIN(002) {
// } AUTOGEN_END

class Activity;
typedef Yb::DomainObjHolder<Activity> ActivityHolder;

class Activity: public Yb::DomainObject
{
public:
    typedef Yb::DomainObjHolder<Activity> Holder;
// AUTOGEN_BEGIN(007) {
    struct Columns {
        Yb::Column id, name, short_name;
        Columns();
        void fill_table(Yb::Table &tbl);
    };
    static Columns c;
// } AUTOGEN_END
    static const Yb::String get_table_name() { return _T("T_ACTIVITY"); }
    typedef Yb::DomainResultSet<Activity> ResultSet;
    // static method 'find'
    typedef std::vector<Activity> List;
    typedef std::auto_ptr<List> ListPtr;
    static ListPtr find(Yb::Session &session,
            const Yb::Expression &filter, const Yb::Expression &order_by = Yb::Expression());
    // constructors
    Activity(Yb::DomainObject *owner, const Yb::String &prop_name);
    Activity();
    Activity(const Activity &other);
    explicit Activity(Yb::Session &session);
    explicit Activity(Yb::DataObject::Ptr d);
    Activity(Yb::Session &session, const Yb::Key &key);
    Activity(Yb::Session &session, Yb::LongInt id);
    Activity &operator=(const Activity &other);
    static void create_tables_meta(Yb::Tables &tbls);
    static void create_relations_meta(Yb::Relations &rels);
// AUTOGEN_BEGIN(003) {
    // properties
    Yb::Property<Yb::LongInt, 0> id;
    Yb::Property<Yb::String, 1> name;
    Yb::Property<Yb::String, 2> short_name;
    // relation properties
// } AUTOGEN_END
};

} // namespace Domain

// vim:ts=4:sts=4:sw=4:et:
#endif
