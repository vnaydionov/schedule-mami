#include "ScheduleControl.h"
#include "domain/ScheduleActivity.h"
#include "domain/ScheduleDate.h"
#include "domain/ScheduleGroup.h"
#include "App.h"

using namespace Yb;
using namespace ElementTree;
using namespace std;
using namespace Domain;

ScheduleControl::ScheduleControl()        
{}

ElementPtr 
ScheduleControl::lastUpdateInfo(Domain::UserHolder user)
{
    Yb::Session *session = user->get_session();
    ElementPtr rt = new_element("last_update_info");
    Schedule::ResultSet rs = query<Schedule>(*session)
        .filter_by(Schedule::c.teacher_id == user->id 
                && Schedule::c.checked == 0).all();
    rs.load();
    Schedule::ResultSet::iterator i = rs.begin(), iend = rs.end();
    if (i == iend) {
        rt->sub_element("status", "false");
        return rt;
    }
    string prev_dt = "",
                dt = "";
    for (; i != iend; ++i) {
        if (i->update_dt.as<string>().compare(prev_dt))
            dt = i->update_dt.as<string>();
        prev_dt = i->update_dt.as<string>();
    }
    rt->sub_element("status", "true");
    rt->sub_element("last_update_dt", dt);
    return rt;
}
 
ElementPtr
ScheduleControl::checkItemsInfo(UserHolder user, const std::string &treeInspections)
{
    Yb::Session *session = user->get_session();
    int      checked   = 0,
             unchecked = 0,
          checked_flag = 0;
    LongInt checked_id = 0;
    ElementPtr rt = new_element("test_check");
    ElementPtr items = rt->sub_element("items");
    ElementPtr ti = parse(treeInspections);
    Schedule::ResultSet rs = query<Schedule>(*session)
        .filter_by(Schedule::c.teacher_id == user->id
                && Schedule::c.checked == 0).all();
    rs.load();
    Schedule::ResultSet::iterator i = rs.begin(), iend = rs.end();
    for (; i != iend; ++i, checked_flag = 0) {
        ElementsPtr id_list = ti->find_all("id");
        Elements::const_iterator j = id_list->begin(), jend = id_list->end();
        for (; j != jend; ++j) {
            checked_id = boost::lexical_cast<LongInt>((*j)->get_text());
            if (i->id == checked_id) {
                if (!(*j)->get_attr("checked").compare("agree")) 
                    checked_flag = 1;
                else   
                    checked_flag = -1;
                i->checked = checked_flag;
                break;
            }
        }
        if (checked_flag == 1) {  //if (i->get_checked())
            ++checked;
            items->sub_element(boost::lexical_cast<string>(checked_id), "agree");
        } else if (checked_flag == -1) {
            ++checked;
            items->sub_element(boost::lexical_cast<string>(checked_id), "disagree");
        } else {
            ++unchecked;
            items->sub_element(boost::lexical_cast<string>(checked_id), "unchecked");
        }
    }
    rt->sub_element("all_checked", boost::lexical_cast<string>(checked));
    rt->sub_element("all_unchecked", boost::lexical_cast<string>(unchecked));
    //session->commit();
    return rt;
}

ElementPtr
ScheduleControl::processRS(Domain::Schedule::ResultSet &rs)
{
    ElementPtr rt = new_element("schedule");
    ElementPtr items = rt->sub_element("items");
    Schedule::ResultSet::iterator i = rs.begin(), iend = rs.end();
    if (i == iend)
        return rt;
    Yb::Session *session = i->get_session();
    for (; i != iend; ++i) {
        items->children_.push_back(createItem(&(*i)));
        i->sent = 1;
    }
    //session->commit();
    return rt;
}

ElementPtr 
ScheduleControl::download(Domain::UserHolder user, const std::string &startDt)
{
    //Yb::ILogger::Ptr log = theApp::instance().new_logger("ScheduleControl:download()"); 
    Yb::Session *session = user->get_session();  
    
    QueryObj<Schedule> expression(*session, Schedule::c.teacher_id == user->id);

    if (!startDt.empty()) {
        DateTime d;
        expression.filter_by(Schedule::c.start_dt >= from_string(startDt, d));
    } else
        expression.filter_by(Schedule::c.sent == 0);

    Schedule::ResultSet rs = expression.all();
    ElementPtr rt = processRS(rs);

    return rt;
}

ElementPtr
ScheduleControl::createItem(Schedule *i)
{
    ElementPtr item = new_element("item");
    item->sub_element("mode", "add"); // and del
    item->sub_element("id", i->id.as<string>());
    item->sub_element("update-dt", i->update_dt.as<string>()); 
    if (i->checked)
        item->sub_element("checked", "true");
    else 
        item->sub_element("checked", "false");
    item->sub_element("place", i->place->name);
    item->sub_element("subject", i->subject->name.value() +
            " (" + i->subject->short_name.value() + ")");
    item->children_.push_back(timeNode(i));
    item->children_.push_back(groupNode(i));
    item->children_.push_back(activityNode(i));
    return item;
}

ElementPtr
ScheduleControl::timeNode(Schedule *i)
{
    ElementPtr time = new_element("time");
    ElementPtr period = time->sub_element("period");
    period->sub_element("repeat", i->repeat_type);
    string dt = i->start_dt.as<string>();
    int  start_erase = dt.find('T'),
         size_erase = dt.size() - start_erase;

    if (i->repeat_type.value().compare("none")) {
        period->sub_element("start", dt.erase(start_erase, size_erase));
        dt = i->end_dt.as<string>();
        period->sub_element("end", dt.erase(start_erase, size_erase));
        period->sub_element("dow", i->dow);
    }
    ElementPtr hours = period->sub_element("hours");
    if (i->start_time/100 < 10)
        hours->sub_element("start", '0' + i->start_time.as<string>());
    else 
        hours->sub_element("start", i->start_time.as<string>());
    if (i->end_time/100 < 10)
        hours->sub_element("end", '0' + i->end_time.as<string>());
    else 
        hours->sub_element("end", i->end_time.as<string>());
    //------excludes&includes----
    ElementPtr excludes = new_element("excludes");
    ElementPtr includes = new_element("includes");
    ManagedList<ScheduleDate>::iterator j = i->dates.begin(),
                                        jend = i->dates.end();
    for (; j != jend; ++j) {
        dt = j->dt.as<string>();
        if (j->item_type == string("exclude"))
            excludes->sub_element("item", dt.erase(start_erase, size_erase));
        else
            includes->sub_element("item", dt.erase(start_erase, size_erase));
    }
    period->children_.push_back(includes);
    period->children_.push_back(excludes);
    return time;
}

ElementPtr
ScheduleControl::groupNode(Schedule *i)
{
    ElementPtr groups = new_element("groups");
    ManagedList<ScheduleGroup>::iterator gr_j = i->groups.begin(),
                                         gr_jend = i->groups.end();
    string t;
    for (; gr_j != gr_jend; ++gr_j) {
        if (!t.empty())
            t += ", ";
        t += gr_j->group->name_code.value() + 
            "(" + gr_j->group->spec_code.value() + ")";
    }
    groups->set_text(t);
    return groups;
}

ElementPtr
ScheduleControl::activityNode(Schedule *i)
{
    ElementPtr activities = new_element("activities");
    ManagedList<ScheduleActivity>::iterator ac_j = i->activities.begin(),
                                            ac_jend = i->activities.end();
    string t;
    for (; ac_j != ac_jend; ++ac_j) {
        if (!t.empty())
            t += ", ";
        t += ac_j->activity->name.value();
    }
    activities->set_text(t);
    return activities; 
}

// vim:ts=4:sts=4:sw=4:et:
