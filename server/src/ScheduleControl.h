#ifndef SCHEDULE_CONTROL__INCLUDED
#define SCHEDULE_CONTROL__INCLUDED

#include "domain/Schedule.h"
#include "domain/User.h"

class ScheduleControl {
    static Yb::ElementTree::ElementPtr createItem(Domain::Schedule *i);
    static Yb::ElementTree::ElementPtr timeNode(Domain::Schedule *i);
    static Yb::ElementTree::ElementPtr groupNode(Domain::Schedule *i);
    static Yb::ElementTree::ElementPtr activityNode(Domain::Schedule *i);
    static Yb::ElementTree::ElementPtr processRS(Domain::Schedule::ResultSet &rs);
    ScheduleControl();
public:
    static Yb::ElementTree::ElementPtr download(Domain::UserHolder user, const std::string &startDt="");
    static Yb::ElementTree::ElementPtr checkItemsInfo(Domain::UserHolder user, const std::string &treeInspections);
    static Yb::ElementTree::ElementPtr lastUpdateInfo(Domain::UserHolder user);
};

#endif
