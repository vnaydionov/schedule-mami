#include "Commands.h"
#include "AuthControl.h"
#include "ScheduleControl.h"
#include "App.h"
#include "domain/LoginSession.h"

std::string
BaseCommand::makeResponse(const std::string &type, const std::string &status,
    Yb::ElementTree::ElementPtr body)
{
    Yb::ElementTree::ElementPtr response = Yb::ElementTree::new_element("response");
    response->attrib_["version"] = "0.1";
    response->attrib_["type"] = type;
    response->sub_element("status", status);
    if (body.get())
        response->children_.push_back(body);
    return "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        + response->serialize();
}

std::string
BaseCommand::makeResponse(const std::string &type, const std::string &status)
{
    Yb::ElementTree::ElementPtr empty_body;
    return makeResponse(type, status, empty_body);
}

AuthCommand::AuthCommand(const StringDict &data):
    BaseCommand(data)
{}

std::string 
AuthCommand::execute()
{
    Yb::LongInt token = AuthControl::login(m_data["login"], m_data["pass"]);
    if (token == -1)
        return makeResponse("auth", "FAILED");
    return makeResponse("auth", "OK", Yb::ElementTree::new_element("token", Yb::to_string(token)));
}

SessionInfoCommand::SessionInfoCommand(const StringDict &data):
    BaseCommand(data)
{}

std::string 
SessionInfoCommand::execute()
{
    Yb::LongInt token;
    Yb::from_string(m_data["token"], token);
    Yb::LongInt sessionId = AuthControl::sessionIsValid(token);
    if (sessionId == -1)
        return makeResponse("session_info", "TOKEN_NOT_EXIST");
    
    std::auto_ptr<Yb::Session> session = theApp::instance().new_session();
    Domain::LoginSession ls(*session, sessionId);
    return makeResponse("session_info", "OK", ls.xmlize());
}

LogoutCommand::LogoutCommand(const StringDict &data):
    BaseCommand(data)
{}

std::string 
LogoutCommand::execute()
{
    Yb::LongInt token;
    Yb::from_string(m_data["token"], token);
    
    if (AuthControl::logout(token))
        return makeResponse("logout", "OK");
    return makeResponse("logout", "FAILED");
}

DownloadScheduleCommand::DownloadScheduleCommand(const StringDict &data):
    BaseCommand(data)
{}

std::string 
DownloadScheduleCommand::execute()
{
    Yb::LongInt token;
    Yb::from_string(m_data["token"], token);
    Yb::LongInt sessionId = AuthControl::sessionIsValid(token);
    if (sessionId == -1)
        return makeResponse("download", "INVALID_SESSION");
    
    std::auto_ptr<Yb::Session> session = theApp::instance().new_session(); 
    Domain::LoginSession ls(*session, sessionId);
    return makeResponse("download", "OK", ScheduleControl::download(ls.user, m_data["start_dt"]));
}

UpdateScheduleCommand::UpdateScheduleCommand(const StringDict &data): 
    BaseCommand(data)
{}

std::string 
UpdateScheduleCommand::execute()
{
    Yb::LongInt token;
    Yb::from_string(m_data["token"], token);
    Yb::LongInt sessionId = AuthControl::sessionIsValid(token);
    if (sessionId == -1)
        return makeResponse("update", "INVALID_SESSION");
    
    std::auto_ptr<Yb::Session> session = theApp::instance().new_session();
    Domain::LoginSession ls(*session, sessionId);
    return makeResponse("update", "OK", ScheduleControl::download(ls.user));
}

CheckScheduleCommand::CheckScheduleCommand(const StringDict &data): 
    BaseCommand(data)
{}

std::string 
CheckScheduleCommand::execute()
{
    Yb::LongInt token;
    Yb::from_string(m_data["token"], token);
    Yb::LongInt sessionId = AuthControl::sessionIsValid(token);
    
    if (sessionId == -1)
        return makeResponse("check", "INVALID_SESSION");
    
    std::auto_ptr<Yb::Session> session = theApp::instance().new_session();
    Domain::LoginSession ls(*session, sessionId);
    return makeResponse("check", "OK", ScheduleControl::download(ls.user, m_data["tree-inspections"]));
}

LastUpdateInfoCommand::LastUpdateInfoCommand(const StringDict &data):
    BaseCommand(data)
{}

std::string 
LastUpdateInfoCommand::execute()
{
    Yb::LongInt token;
    Yb::from_string(m_data["token"], token);
    Yb::LongInt sessionId = AuthControl::sessionIsValid(token);
    
    if (sessionId == -1)
        return makeResponse("last_update_info", "INVALID_SESSION");
    
    std::auto_ptr<Yb::Session> session = theApp::instance().new_session();
    Domain::LoginSession ls(*session, sessionId);
    return makeResponse("last_update_info", "OK", ScheduleControl::lastUpdateInfo(ls.user));
}

