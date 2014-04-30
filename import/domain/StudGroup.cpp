#include "domain/StudGroup.h"
#include <orm/DomainFactory.h>
namespace Domain {

StudGroup::Columns StudGroup::c;

namespace {
    Yb::Tables tbls;
    Yb::Relations rels;
    Yb::DomainMetaDataCreator<StudGroup> mdc(tbls, rels);
}

// AUTOGEN_BEGIN(008) {
StudGroup::Columns::Columns()
    : id(_T("ID"), Yb::Value::LONGINT, 0, Yb::Column::PK, Yb::Value(), _T(""), _T(""), _T("id"), _T("id"))
    , spec_code(_T("SPEC_CODE"), Yb::Value::STRING, 20, 0, Yb::Value(), _T(""), _T(""), _T("spec-code"), _T("spec_code"))
    , year(_T("YEAR"), Yb::Value::INTEGER, 0, 0, Yb::Value(), _T(""), _T(""), _T("year"), _T("year"))
    , name_code(_T("NAME_CODE"), Yb::Value::STRING, 20, 0, Yb::Value(), _T(""), _T(""), _T("name-code"), _T("name_code"))
    , fulltime(_T("FULLTIME"), Yb::Value::INTEGER, 0, 0, Yb::Value(), _T(""), _T(""), _T("fulltime"), _T("fulltime"))
    , short_(_T("SHORT"), Yb::Value::INTEGER, 0, 0, Yb::Value(), _T(""), _T(""), _T("short"), _T("short"))
    , grad_type(_T("GRAD_TYPE"), Yb::Value::INTEGER, 0, 0, Yb::Value(), _T(""), _T(""), _T("grad-type"), _T("grad_type"))
{}

void StudGroup::Columns::fill_table(Yb::Table &tbl)
{
    tbl << id << spec_code << year << name_code << fulltime << short_ << grad_type;
}

void StudGroup::create_tables_meta(Yb::Tables &tbls)
{
    Yb::Table::Ptr t(new Yb::Table(_T("T_STUD_GROUP"), _T("stud-group"), _T("StudGroup")));
    t->set_seq_name(_T("S_STUD_GROUP"));
    c.fill_table(*t);
    tbls.push_back(t);
}

void StudGroup::create_relations_meta(Yb::Relations &rels)
{
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr2[_T("property")] = _T("group");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("StudGroup"), attr1, _T("ScheduleGroup"), attr2, Yb::Relation::Delete));
        rels.push_back(r);
    }
}
// } AUTOGEN_END

StudGroup::StudGroup(Yb::DomainObject *owner, const Yb::String &prop_name)
    : Yb::DomainObject(*tbls[0], owner, prop_name)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , spec_code(this)
    , year(this)
    , name_code(this)
    , fulltime(this)
    , short_(this)
    , grad_type(this)
// } AUTOGEN_END
{}

StudGroup::StudGroup()
    : Yb::DomainObject(*tbls[0])
// AUTOGEN_BEGIN(005) {
    , id(this)
    , spec_code(this)
    , year(this)
    , name_code(this)
    , fulltime(this)
    , short_(this)
    , grad_type(this)
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
}

StudGroup::StudGroup(const StudGroup &other)
    : Yb::DomainObject(other)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , spec_code(this)
    , year(this)
    , name_code(this)
    , fulltime(this)
    , short_(this)
    , grad_type(this)
// } AUTOGEN_END
{}

StudGroup::StudGroup(Yb::Session &session)
    : Yb::DomainObject(session.schema(), _T("T_STUD_GROUP"))
// AUTOGEN_BEGIN(005) {
    , id(this)
    , spec_code(this)
    , year(this)
    , name_code(this)
    , fulltime(this)
    , short_(this)
    , grad_type(this)
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
    save(session);
}

StudGroup::StudGroup(Yb::DataObject::Ptr d)
    : Yb::DomainObject(d)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , spec_code(this)
    , year(this)
    , name_code(this)
    , fulltime(this)
    , short_(this)
    , grad_type(this)
// } AUTOGEN_END
{}

StudGroup::StudGroup(Yb::Session &session, const Yb::Key &key)
    : Yb::DomainObject(session, key)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , spec_code(this)
    , year(this)
    , name_code(this)
    , fulltime(this)
    , short_(this)
    , grad_type(this)
// } AUTOGEN_END
{}

StudGroup::StudGroup(Yb::Session &session, Yb::LongInt id)
    : Yb::DomainObject(session, _T("T_STUD_GROUP"), id)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , spec_code(this)
    , year(this)
    , name_code(this)
    , fulltime(this)
    , short_(this)
    , grad_type(this)
// } AUTOGEN_END
{}

StudGroup &StudGroup::operator=(const StudGroup &other)
{
    if (this != &other) {
        *(Yb::DomainObject *)this = (const Yb::DomainObject &)other;
    }
    return *this;
}

StudGroup::ListPtr
StudGroup::find(Yb::Session &session,
        const Yb::Expression &filter, const Yb::Expression &order_by)
{
    StudGroup::ListPtr lst(new StudGroup::List());
    Yb::ObjectList rows;
    session.load_collection(rows, Yb::Expression(_T("T_STUD_GROUP")), filter, order_by);
    if (rows.size()) {
        Yb::ObjectList::iterator it = rows.begin(), end = rows.end();
        for (; it != end; ++it)
            lst->push_back(StudGroup(*it));
    }
    return lst;
}

struct StudGroupRegistrator
{
    static void register_domain() {
        Yb::theDomainFactory::instance().register_creator(_T("T_STUD_GROUP"),
            Yb::CreatorPtr(new Yb::DomainCreator<StudGroup>()));
    }
    StudGroupRegistrator() { register_domain(); }
};

static StudGroupRegistrator StudGroup_registrator;

} // end namespace Domain

// vim:ts=4:sts=4:sw=4:et:
