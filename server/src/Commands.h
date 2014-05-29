#ifndef COMMANDS_H
#define COMMANDS_H

#include <orm/data_object.h>
#include <orm/xmlizer.h>

typedef std::map<std::string, std::string> StringMap;

class BaseCommand {      
public:
    BaseCommand(const StringMap &data):
        m_data(data)
    {}
    virtual ~BaseCommand() {}
    virtual std::string execute() = 0;
protected:
    std::string makeResponse(const std::string &type, const std::string &status, Yb::ElementTree::ElementPtr body);
    std::string makeResponse(const std::string &type, const std::string &status);
    StringMap m_data;
};

class AuthCommand: public BaseCommand {
public:
    AuthCommand(const StringMap &data); 
    std::string execute();
};

class SessionInfoCommand: public BaseCommand {
public:
    SessionInfoCommand(const StringMap &data); 
    std::string execute();
};

class LogoutCommand: public BaseCommand {
public:
    LogoutCommand(const StringMap &data); 
    std::string execute();
};

class UpdateScheduleCommand: public BaseCommand {
public:
    UpdateScheduleCommand(const StringMap &data); 
    std::string execute();
};

class DownloadScheduleCommand: public BaseCommand {
public:
    DownloadScheduleCommand(const StringMap &data); 
    std::string execute();
};

class CheckScheduleCommand: public BaseCommand {
public:
    CheckScheduleCommand(const StringMap &data); 
    std::string execute();
};

class LastUpdateInfoCommand: public BaseCommand {
public:
    LastUpdateInfoCommand(const StringMap &data); 
    std::string execute();
};

#endif // COMMANDS_H
