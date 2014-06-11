#ifndef ORM_DOMAIN__SUBJECT__INCLUDED
#define ORM_DOMAIN__SUBJECT__INCLUDED

#include <orm/DomainObject.h>
// AUTOGEN_BEGIN(001) {
// } AUTOGEN_END

namespace Domain {

// AUTOGEN_BEGIN(002) {
class Schedule;
typedef Yb::DomainObjHolder<Schedule> ScheduleHolder;
// } AUTOGEN_END

class Subject;
typedef Yb::DomainObjHolder<Subject> SubjectHolder;

class Subject: public Yb::DomainObject
{
public:
    typedef Yb::DomainObjHolder<Subject> Holder;
// AUTOGEN_BEGIN(007) {
    struct Columns {
        Yb::Column id, name, short_name;
        Columns();
        void fill_table(Yb::Table &tbl);
    };
    static Columns c;
// } AUTOGEN_END
    static const Yb::String get_table_name() { return _T("T_SUBJECT"); }
    typedef Yb::DomainResultSet<Subject> ResultSet;
    // static method 'find'
    typedef std::vector<Subject> List;
    typedef std::auto_ptr<List> ListPtr;
    static ListPtr find(Yb::Session &session,
            const Yb::Expression &filter, const Yb::Expression &order_by = Yb::Expression());
    // constructors
    Subject(Yb::DomainObject *owner, const Yb::String &prop_name);
    Subject();
    Subject(const Subject &other);
    explicit Subject(Yb::Session &session);
    explicit Subject(Yb::DataObject::Ptr d);
    Subject(Yb::Session &session, const Yb::Key &key);
    Subject(Yb::Session &session, Yb::LongInt id);
    Subject &operator=(const Subject &other);
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
