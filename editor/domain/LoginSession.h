#ifndef ORM_DOMAIN__LOGINSESSION__INCLUDED
#define ORM_DOMAIN__LOGINSESSION__INCLUDED

#include <orm/domain_object.h>
// AUTOGEN_BEGIN(001) {
#include "domain/User.h"
// } AUTOGEN_END

namespace Domain {

// AUTOGEN_BEGIN(002) {
// } AUTOGEN_END

class LoginSession;
typedef Yb::DomainObjHolder<LoginSession> LoginSessionHolder;

class LoginSession: public Yb::DomainObject
{
public:
    typedef Yb::DomainObjHolder<LoginSession> Holder;
// AUTOGEN_BEGIN(007) {
    struct Columns {
        Yb::Column id, usr_id, app_name, begin_session, end_session, token;
        Columns();
        void fill_table(Yb::Table &tbl);
    };
    static Columns c;
// } AUTOGEN_END
    static const Yb::String get_table_name() { return _T("T_SESSION"); }
    typedef Yb::DomainResultSet<LoginSession> ResultSet;
    // static method 'find'
    typedef std::vector<LoginSession> List;
    typedef std::auto_ptr<List> ListPtr;
    static ListPtr find(Yb::Session &session,
            const Yb::Expression &filter, const Yb::Expression &order_by = Yb::Expression());
    // constructors
    LoginSession(Yb::DomainObject *owner, const Yb::String &prop_name);
    LoginSession();
    LoginSession(const LoginSession &other);
    explicit LoginSession(Yb::Session &session);
    explicit LoginSession(Yb::DataObject::Ptr d);
    LoginSession(Yb::Session &session, const Yb::Key &key);
    LoginSession(Yb::Session &session, Yb::LongInt id);
    LoginSession &operator=(const LoginSession &other);
    static void create_tables_meta(Yb::Tables &tbls);
    static void create_relations_meta(Yb::Relations &rels);
// AUTOGEN_BEGIN(003) {
    // properties
    Yb::Property<Yb::LongInt, 0> id;
    Yb::Property<Yb::String, 2> app_name;
    Yb::Property<Yb::DateTime, 3> begin_session;
    Yb::Property<Yb::DateTime, 4> end_session;
    Yb::Property<Yb::String, 5> token;
    // relation properties
    User::Holder user;
// } AUTOGEN_END
};

} // namespace Domain

// vim:ts=4:sts=4:sw=4:et:
#endif
