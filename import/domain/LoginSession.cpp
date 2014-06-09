#include "domain/LoginSession.h"
#include <orm/domain_factory.h>
namespace Domain {

LoginSession::Columns LoginSession::c;

namespace {
    Yb::Tables tbls;
    Yb::Relations rels;
    Yb::DomainMetaDataCreator<LoginSession> mdc(tbls, rels);
}

// AUTOGEN_BEGIN(008) {
LoginSession::Columns::Columns()
    : id(_T("ID"), Yb::Value::LONGINT, 0, Yb::Column::PK, Yb::Value(), _T(""), _T(""), _T("id"), _T("id"))
    , usr_id(_T("USR_ID"), Yb::Value::LONGINT, 0, 0, Yb::Value(), _T("T_USER"), _T("ID"), _T("usr-id"), _T("usr_id"))
    , app_name(_T("APP_NAME"), Yb::Value::STRING, 100, 0, Yb::Value(), _T(""), _T(""), _T("app-name"), _T("app_name"))
    , begin_session(_T("BEGIN_SESSION"), Yb::Value::DATETIME, 0, 0, Yb::Value(_T("sysdate")), _T(""), _T(""), _T("begin-session"), _T("begin_session"))
    , end_session(_T("END_SESSION"), Yb::Value::DATETIME, 0, Yb::Column::NULLABLE, Yb::Value(), _T(""), _T(""), _T("end-session"), _T("end_session"))
    , token(_T("TOKEN"), Yb::Value::STRING, 100, Yb::Column::NULLABLE, Yb::Value(), _T(""), _T(""), _T("token"), _T("token"))
{}

void LoginSession::Columns::fill_table(Yb::Table &tbl)
{
    tbl << id << usr_id << app_name << begin_session << end_session << token;
}

void LoginSession::create_tables_meta(Yb::Tables &tbls)
{
    Yb::Table::Ptr t(new Yb::Table(_T("T_SESSION"), _T("login-session"), _T("LoginSession")));
    t->set_seq_name(_T("S_SESSION"));
    c.fill_table(*t);
    tbls.push_back(t);
}

void LoginSession::create_relations_meta(Yb::Relations &rels)
{
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("login_sessions");
        attr2[_T("property")] = _T("user");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("User"), attr1, _T("LoginSession"), attr2, Yb::Relation::Restrict));
        rels.push_back(r);
    }
}
// } AUTOGEN_END

LoginSession::LoginSession(Yb::DomainObject *owner, const Yb::String &prop_name)
    : Yb::DomainObject(*tbls[0], owner, prop_name)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , app_name(this)
    , begin_session(this)
    , end_session(this)
    , token(this)
    , user(this, _T("user"))
// } AUTOGEN_END
{}

LoginSession::LoginSession()
    : Yb::DomainObject(*tbls[0])
// AUTOGEN_BEGIN(005) {
    , id(this)
    , app_name(this)
    , begin_session(this)
    , end_session(this)
    , token(this)
    , user(this, _T("user"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
    set(3, Yb::Value(Yb::now()));
// } AUTOGEN_END
}

LoginSession::LoginSession(const LoginSession &other)
    : Yb::DomainObject(other)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , app_name(this)
    , begin_session(this)
    , end_session(this)
    , token(this)
    , user(this, _T("user"))
// } AUTOGEN_END
{}

LoginSession::LoginSession(Yb::Session &session)
    : Yb::DomainObject(session.schema(), _T("T_SESSION"))
// AUTOGEN_BEGIN(005) {
    , id(this)
    , app_name(this)
    , begin_session(this)
    , end_session(this)
    , token(this)
    , user(this, _T("user"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
    set(3, Yb::Value(Yb::now()));
// } AUTOGEN_END
    save(session);
}

LoginSession::LoginSession(Yb::DataObject::Ptr d)
    : Yb::DomainObject(d)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , app_name(this)
    , begin_session(this)
    , end_session(this)
    , token(this)
    , user(this, _T("user"))
// } AUTOGEN_END
{}

LoginSession::LoginSession(Yb::Session &session, const Yb::Key &key)
    : Yb::DomainObject(session, key)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , app_name(this)
    , begin_session(this)
    , end_session(this)
    , token(this)
    , user(this, _T("user"))
// } AUTOGEN_END
{}

LoginSession::LoginSession(Yb::Session &session, Yb::LongInt id)
    : Yb::DomainObject(session, _T("T_SESSION"), id)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , app_name(this)
    , begin_session(this)
    , end_session(this)
    , token(this)
    , user(this, _T("user"))
// } AUTOGEN_END
{}

LoginSession &LoginSession::operator=(const LoginSession &other)
{
    if (this != &other) {
        *(Yb::DomainObject *)this = (const Yb::DomainObject &)other;
    }
    return *this;
}

LoginSession::ListPtr
LoginSession::find(Yb::Session &session,
        const Yb::Expression &filter, const Yb::Expression &order_by)
{
    LoginSession::ListPtr lst(new LoginSession::List());
    Yb::ObjectList rows;
    session.load_collection(rows, Yb::Expression(_T("T_SESSION")), filter, order_by);
    if (rows.size()) {
        Yb::ObjectList::iterator it = rows.begin(), end = rows.end();
        for (; it != end; ++it)
            lst->push_back(LoginSession(*it));
    }
    return lst;
}

struct LoginSessionRegistrator
{
    static void register_domain() {
        Yb::theDomainFactory().register_creator(_T("T_SESSION"),
            Yb::CreatorPtr(new Yb::DomainCreator<LoginSession>()));
    }
    LoginSessionRegistrator() { register_domain(); }
};

static LoginSessionRegistrator LoginSession_registrator;

} // end namespace Domain

// vim:ts=4:sts=4:sw=4:et:
