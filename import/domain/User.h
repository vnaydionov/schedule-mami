#ifndef ORM_DOMAIN__USER__INCLUDED
#define ORM_DOMAIN__USER__INCLUDED

#include <orm/DomainObject.h>
// AUTOGEN_BEGIN(001) {
// } AUTOGEN_END

namespace Domain {

// AUTOGEN_BEGIN(002) {
class LoginSession;
typedef Yb::DomainObjHolder<LoginSession> LoginSessionHolder;
class Schedule;
typedef Yb::DomainObjHolder<Schedule> ScheduleHolder;
// } AUTOGEN_END

class User;
typedef Yb::DomainObjHolder<User> UserHolder;

class User: public Yb::DomainObject
{
public:
    typedef Yb::DomainObjHolder<User> Holder;
// AUTOGEN_BEGIN(007) {
    struct Columns {
        Yb::Column id, name, pass, email, login, status, facult, is_superuser, phone;
        Columns();
        void fill_table(Yb::Table &tbl);
    };
    static Columns c;
// } AUTOGEN_END
    static const Yb::String get_table_name() { return _T("T_USER"); }
    typedef Yb::DomainResultSet<User> ResultSet;
    // static method 'find'
    typedef std::vector<User> List;
    typedef std::auto_ptr<List> ListPtr;
    static ListPtr find(Yb::Session &session,
            const Yb::Expression &filter, const Yb::Expression &order_by = Yb::Expression());
    // constructors
    User(Yb::DomainObject *owner, const Yb::String &prop_name);
    User();
    User(const User &other);
    explicit User(Yb::Session &session);
    explicit User(Yb::DataObject::Ptr d);
    User(Yb::Session &session, const Yb::Key &key);
    User(Yb::Session &session, Yb::LongInt id);
    User &operator=(const User &other);
    static void create_tables_meta(Yb::Tables &tbls);
    static void create_relations_meta(Yb::Relations &rels);
// AUTOGEN_BEGIN(003) {
    // properties
    Yb::Property<Yb::LongInt, 0> id;
    Yb::Property<Yb::String, 1> name;
    Yb::Property<Yb::String, 2> pass;
    Yb::Property<Yb::String, 3> email;
    Yb::Property<Yb::String, 4> login;
    Yb::Property<Yb::LongInt, 5> status;
    Yb::Property<Yb::LongInt, 6> facult;
    Yb::Property<Yb::LongInt, 7> is_superuser;
    Yb::Property<Yb::String, 8> phone;
    // relation properties
    Yb::ManagedList<LoginSession> login_sessions;
    Yb::ManagedList<Schedule> classes;
    Yb::ManagedList<Schedule> subscriptions;
// } AUTOGEN_END
};

} // namespace Domain

// vim:ts=4:sts=4:sw=4:et:
#endif
