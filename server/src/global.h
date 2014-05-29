#ifndef GLOBAL_H
#define GLOBAL_H

#include "Commands.h"
#include <string>
#include <map>

enum RequestTypes {
    Auth = 1,
    SessionInfo,
    Logout,
    UpdateSchedule,
    DownloadSchedule,
    CheckSchedule,
    LastUpdateInfo,
    UnknownType
};

#endif // GLOBAL_H

// vim:ts=4:sts=4:sw=4:et:
