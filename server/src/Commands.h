#ifndef COMMANDS_H
#define COMMANDS_H

#include <util/data_types.h>
#include <orm/data_object.h>
#include <orm/xmlizer.h>

using ::Yb::StringDict;

class BaseCommand {      
public:
    BaseCommand(const StringDict &data):
        m_data(data)
    {}
    virtual ~BaseCommand() {}
    virtual std::string execute() = 0;
protected:
    std::string makeResponse(const std::string &type, const std::string &status, Yb::ElementTree::ElementPtr body);
    std::string makeResponse(const std::string &type, const std::string &status);
    StringDict m_data;
};

class AuthCommand: public BaseCommand {
public:
    AuthCommand(const StringDict &data); 
    std::string execute();
};

class SessionInfoCommand: public BaseCommand {
public:
    SessionInfoCommand(const StringDict &data); 
    std::string execute();
};

class LogoutCommand: public BaseCommand {
public:
    LogoutCommand(const StringDict &data); 
    std::string execute();
};

class UpdateScheduleCommand: public BaseCommand {
public:
    UpdateScheduleCommand(const StringDict &data); 
    std::string execute();
};

class DownloadScheduleCommand: public BaseCommand {
public:
    DownloadScheduleCommand(const StringDict &data); 
    std::string execute();
};

class CheckScheduleCommand: public BaseCommand {
public:
    CheckScheduleCommand(const StringDict &data); 
    std::string execute();
};

class LastUpdateInfoCommand: public BaseCommand {
public:
    LastUpdateInfoCommand(const StringDict &data); 
    std::string execute();
};

#endif // COMMANDS_H
