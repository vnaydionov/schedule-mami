#include "domain/User.h"
#include <orm/DomainFactory.h>
namespace Domain {

User::Columns User::c;

namespace {
    Yb::Tables tbls;
    Yb::Relations rels;
    Yb::DomainMetaDataCreator<User> mdc(tbls, rels);
}

// AUTOGEN_BEGIN(008) {
User::Columns::Columns()
    : id(_T("ID"), Yb::Value::LONGINT, 0, Yb::Column::PK, Yb::Value(), _T(""), _T(""), _T("id"), _T("id"))
    , name(_T("NAME"), Yb::Value::STRING, 100, 0, Yb::Value(), _T(""), _T(""), _T("name"), _T("name"))
    , pass(_T("PASS"), Yb::Value::STRING, 100, 0, Yb::Value(), _T(""), _T(""), _T("!"), _T("pass"))
    , email(_T("EMAIL"), Yb::Value::STRING, 40, Yb::Column::NULLABLE, Yb::Value(), _T(""), _T(""), _T("email"), _T("email"))
    , login(_T("LOGIN"), Yb::Value::STRING, 40, Yb::Column::NULLABLE, Yb::Value(), _T(""), _T(""), _T("login"), _T("login"))
    , status(_T("STATUS"), Yb::Value::LONGINT, 0, 0, Yb::Value(_T("1")), _T(""), _T(""), _T("status"), _T("status"))
    , facult(_T("FACULT"), Yb::Value::LONGINT, 0, 0, Yb::Value(_T("1")), _T(""), _T(""), _T("facult"), _T("facult"))
    , is_superuser(_T("IS_SUPERUSER"), Yb::Value::LONGINT, 0, 0, Yb::Value(_T("0")), _T(""), _T(""), _T("!"), _T("is_superuser"))
    , phone(_T("PHONE"), Yb::Value::STRING, 40, Yb::Column::NULLABLE, Yb::Value(), _T(""), _T(""), _T("phone"), _T("phone"))
{}

void User::Columns::fill_table(Yb::Table &tbl)
{
    tbl << id << name << pass << email << login << status << facult << is_superuser << phone;
}

void User::create_tables_meta(Yb::Tables &tbls)
{
    Yb::Table::Ptr t(new Yb::Table(_T("T_USER"), _T("user"), _T("User")));
    t->set_seq_name(_T("S_USER"));
    c.fill_table(*t);
    tbls.push_back(t);
}

void User::create_relations_meta(Yb::Relations &rels)
{
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("login_sessions");
        attr2[_T("property")] = _T("user");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("User"), attr1, _T("LoginSession"), attr2, Yb::Relation::Restrict));
        rels.push_back(r);
    }
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("classes");
        attr2[_T("key")] = _T("TEACHER_ID");
        attr2[_T("property")] = _T("teacher");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("User"), attr1, _T("Schedule"), attr2, Yb::Relation::Restrict));
        rels.push_back(r);
    }
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("subscriptions");
        attr2[_T("key")] = _T("RECEIVER_ID");
        attr2[_T("property")] = _T("receiver");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("User"), attr1, _T("Schedule"), attr2, Yb::Relation::Restrict));
        rels.push_back(r);
    }
}
// } AUTOGEN_END

User::User(Yb::DomainObject *owner, const Yb::String &prop_name)
    : Yb::DomainObject(*tbls[0], owner, prop_name)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , pass(this)
    , email(this)
    , login(this)
    , status(this)
    , facult(this)
    , is_superuser(this)
    , phone(this)
    , login_sessions(this, _T("login_sessions"))
    , classes(this, _T("classes"))
    , subscriptions(this, _T("subscriptions"))
// } AUTOGEN_END
{}

User::User()
    : Yb::DomainObject(*tbls[0])
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , pass(this)
    , email(this)
    , login(this)
    , status(this)
    , facult(this)
    , is_superuser(this)
    , phone(this)
    , login_sessions(this, _T("login_sessions"))
    , classes(this, _T("classes"))
    , subscriptions(this, _T("subscriptions"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
    set(5, Yb::Value((Yb::LongInt)1));
    set(6, Yb::Value((Yb::LongInt)1));
    set(7, Yb::Value((Yb::LongInt)0));
// } AUTOGEN_END
}

User::User(const User &other)
    : Yb::DomainObject(other)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , pass(this)
    , email(this)
    , login(this)
    , status(this)
    , facult(this)
    , is_superuser(this)
    , phone(this)
    , login_sessions(this, _T("login_sessions"))
    , classes(this, _T("classes"))
    , subscriptions(this, _T("subscriptions"))
// } AUTOGEN_END
{}

User::User(Yb::Session &session)
    : Yb::DomainObject(session.schema(), _T("T_USER"))
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , pass(this)
    , email(this)
    , login(this)
    , status(this)
    , facult(this)
    , is_superuser(this)
    , phone(this)
    , login_sessions(this, _T("login_sessions"))
    , classes(this, _T("classes"))
    , subscriptions(this, _T("subscriptions"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
    set(5, Yb::Value((Yb::LongInt)1));
    set(6, Yb::Value((Yb::LongInt)1));
    set(7, Yb::Value((Yb::LongInt)0));
// } AUTOGEN_END
    save(session);
}

User::User(Yb::DataObject::Ptr d)
    : Yb::DomainObject(d)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , pass(this)
    , email(this)
    , login(this)
    , status(this)
    , facult(this)
    , is_superuser(this)
    , phone(this)
    , login_sessions(this, _T("login_sessions"))
    , classes(this, _T("classes"))
    , subscriptions(this, _T("subscriptions"))
// } AUTOGEN_END
{}

User::User(Yb::Session &session, const Yb::Key &key)
    : Yb::DomainObject(session, key)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , pass(this)
    , email(this)
    , login(this)
    , status(this)
    , facult(this)
    , is_superuser(this)
    , phone(this)
    , login_sessions(this, _T("login_sessions"))
    , classes(this, _T("classes"))
    , subscriptions(this, _T("subscriptions"))
// } AUTOGEN_END
{}

User::User(Yb::Session &session, Yb::LongInt id)
    : Yb::DomainObject(session, _T("T_USER"), id)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , pass(this)
    , email(this)
    , login(this)
    , status(this)
    , facult(this)
    , is_superuser(this)
    , phone(this)
    , login_sessions(this, _T("login_sessions"))
    , classes(this, _T("classes"))
    , subscriptions(this, _T("subscriptions"))
// } AUTOGEN_END
{}

User &User::operator=(const User &other)
{
    if (this != &other) {
        *(Yb::DomainObject *)this = (const Yb::DomainObject &)other;
    }
    return *this;
}

User::ListPtr
User::find(Yb::Session &session,
        const Yb::Expression &filter, const Yb::Expression &order_by)
{
    User::ListPtr lst(new User::List());
    Yb::ObjectList rows;
    session.load_collection(rows, Yb::Expression(_T("T_USER")), filter, order_by);
    if (rows.size()) {
        Yb::ObjectList::iterator it = rows.begin(), end = rows.end();
        for (; it != end; ++it)
            lst->push_back(User(*it));
    }
    return lst;
}

struct UserRegistrator
{
    static void register_domain() {
        Yb::theDomainFactory::instance().register_creator(_T("T_USER"),
            Yb::CreatorPtr(new Yb::DomainCreator<User>()));
    }
    UserRegistrator() { register_domain(); }
};

static UserRegistrator User_registrator;

} // end namespace Domain

// vim:ts=4:sts=4:sw=4:et:
