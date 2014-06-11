#ifndef ORM_DOMAIN__STUDGROUP__INCLUDED
#define ORM_DOMAIN__STUDGROUP__INCLUDED

#include <orm/DomainObject.h>
// AUTOGEN_BEGIN(001) {
// } AUTOGEN_END

namespace Domain {

// AUTOGEN_BEGIN(002) {
// } AUTOGEN_END

class StudGroup;
typedef Yb::DomainObjHolder<StudGroup> StudGroupHolder;

class StudGroup: public Yb::DomainObject
{
public:
    typedef Yb::DomainObjHolder<StudGroup> Holder;
// AUTOGEN_BEGIN(007) {
    struct Columns {
        Yb::Column id, spec_code, year, name_code, fulltime, short_, grad_type;
        Columns();
        void fill_table(Yb::Table &tbl);
    };
    static Columns c;
// } AUTOGEN_END
    static const Yb::String get_table_name() { return _T("T_STUD_GROUP"); }
    typedef Yb::DomainResultSet<StudGroup> ResultSet;
    // static method 'find'
    typedef std::vector<StudGroup> List;
    typedef std::auto_ptr<List> ListPtr;
    static ListPtr find(Yb::Session &session,
            const Yb::Expression &filter, const Yb::Expression &order_by = Yb::Expression());
    // constructors
    StudGroup(Yb::DomainObject *owner, const Yb::String &prop_name);
    StudGroup();
    StudGroup(const StudGroup &other);
    explicit StudGroup(Yb::Session &session);
    explicit StudGroup(Yb::DataObject::Ptr d);
    StudGroup(Yb::Session &session, const Yb::Key &key);
    StudGroup(Yb::Session &session, Yb::LongInt id);
    StudGroup &operator=(const StudGroup &other);
    static void create_tables_meta(Yb::Tables &tbls);
    static void create_relations_meta(Yb::Relations &rels);
// AUTOGEN_BEGIN(003) {
    // properties
    Yb::Property<Yb::LongInt, 0> id;
    Yb::Property<Yb::String, 1> spec_code;
    Yb::Property<int, 2> year;
    Yb::Property<Yb::String, 3> name_code;
    Yb::Property<int, 4> fulltime;
    Yb::Property<int, 5> short_;
    Yb::Property<int, 6> grad_type;
    // relation properties
// } AUTOGEN_END
};

} // namespace Domain

// vim:ts=4:sts=4:sw=4:et:
#endif
