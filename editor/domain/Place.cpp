#include "domain/Place.h"
#include <orm/domain_factory.h>
namespace Domain {

Place::Columns Place::c;

namespace {
    Yb::Tables tbls;
    Yb::Relations rels;
    Yb::DomainMetaDataCreator<Place> mdc(tbls, rels);
}

// AUTOGEN_BEGIN(008) {
Place::Columns::Columns()
    : id(_T("ID"), Yb::Value::LONGINT, 0, Yb::Column::PK, Yb::Value(), _T(""), _T(""), _T("id"), _T("id"))
    , name(_T("NAME"), Yb::Value::STRING, 100, 0, Yb::Value(), _T(""), _T(""), _T("name"), _T("name"))
    , short_name(_T("SHORT_NAME"), Yb::Value::STRING, 20, 0, Yb::Value(), _T(""), _T(""), _T("short-name"), _T("short_name"))
{}

void Place::Columns::fill_table(Yb::Table &tbl)
{
    tbl << id << name << short_name;
}

void Place::create_tables_meta(Yb::Tables &tbls)
{
    Yb::Table::Ptr t(new Yb::Table(_T("T_PLACE"), _T("place"), _T("Place")));
    t->set_seq_name(_T("S_PLACE"));
    c.fill_table(*t);
    tbls.push_back(t);
}

void Place::create_relations_meta(Yb::Relations &rels)
{
    {
        Yb::Relation::AttrMap attr1, attr2;
        attr1[_T("property")] = _T("classes");
        attr2[_T("property")] = _T("place");
        Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, _T("Place"), attr1, _T("Schedule"), attr2, Yb::Relation::Restrict));
        rels.push_back(r);
    }
}
// } AUTOGEN_END

Place::Place(Yb::DomainObject *owner, const Yb::String &prop_name)
    : Yb::DomainObject(*tbls[0], owner, prop_name)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
    , classes(this, _T("classes"))
// } AUTOGEN_END
{}

Place::Place()
    : Yb::DomainObject(*tbls[0])
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
    , classes(this, _T("classes"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
}

Place::Place(const Place &other)
    : Yb::DomainObject(other)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
    , classes(this, _T("classes"))
// } AUTOGEN_END
{}

Place::Place(Yb::Session &session)
    : Yb::DomainObject(session.schema(), _T("T_PLACE"))
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
    , classes(this, _T("classes"))
// } AUTOGEN_END
{
// AUTOGEN_BEGIN(006) {
// } AUTOGEN_END
    save(session);
}

Place::Place(Yb::DataObject::Ptr d)
    : Yb::DomainObject(d)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
    , classes(this, _T("classes"))
// } AUTOGEN_END
{}

Place::Place(Yb::Session &session, const Yb::Key &key)
    : Yb::DomainObject(session, key)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
    , classes(this, _T("classes"))
// } AUTOGEN_END
{}

Place::Place(Yb::Session &session, Yb::LongInt id)
    : Yb::DomainObject(session, _T("T_PLACE"), id)
// AUTOGEN_BEGIN(005) {
    , id(this)
    , name(this)
    , short_name(this)
    , classes(this, _T("classes"))
// } AUTOGEN_END
{}

Place &Place::operator=(const Place &other)
{
    if (this != &other) {
        *(Yb::DomainObject *)this = (const Yb::DomainObject &)other;
    }
    return *this;
}

Place::ListPtr
Place::find(Yb::Session &session,
        const Yb::Expression &filter, const Yb::Expression &order_by)
{
    Place::ListPtr lst(new Place::List());
    Yb::ObjectList rows;
    session.load_collection(rows, Yb::Expression(_T("T_PLACE")), filter, order_by);
    if (rows.size()) {
        Yb::ObjectList::iterator it = rows.begin(), end = rows.end();
        for (; it != end; ++it)
            lst->push_back(Place(*it));
    }
    return lst;
}

struct PlaceRegistrator
{
    static void register_domain() {
        Yb::theDomainFactory().register_creator(_T("T_PLACE"),
            Yb::CreatorPtr(new Yb::DomainCreator<Place>()));
    }
    PlaceRegistrator() { register_domain(); }
};

static PlaceRegistrator Place_registrator;

} // end namespace Domain

// vim:ts=4:sts=4:sw=4:et:
