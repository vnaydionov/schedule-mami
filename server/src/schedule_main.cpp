#if defined(__WIN32__) || defined(_WIN32)
#include <rpc.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif
#include <boost/tuple/tuple.hpp>
#include "App.h"
#include "micro_http.h"
#include "domain/Activity.h"
#include "domain/ScheduleActivity.h"
#include "domain/Schedule.h"
#include "ContentChecker.h"
#include "Commands.h"
#include "global.h"

using namespace std;
using namespace Domain;
using namespace Yb::ElementTree;

Yb::ILogger::Ptr logger;

string
etree2str(ElementPtr xml)
{
    return "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" + xml->serialize();
}

/*
string 
adm_login(StringMap &params)
{      
    Responsible resp(new Requester(AUTH, params, 1, SCHEDULE_ADM));
    resp.process_request();
    return resp.get_response();
}


string 
adm_session_info(StringMap &params)
{
    Responsible resp(new Requester(SESSION_INFO, params, 1, SCHEDULE_ADM));
    resp.process_request();
    return resp.get_response();
}

string 
adm_logout(StringMap &params)
{ 
    Responsible resp(new Requester(LOGOUT, params, 1, SCHEDULE_ADM));
    resp.process_request();
    return resp.get_response();
}

string
adm_add_activity(StringMap &params)
{
    std::auto_ptr<Yb::Session> session(theApp::instance().new_session());
    AuthControl auth(new Requester(0, params, 1, SCHEDULE_ADM));
    if (!auth.session_is_valid())
        return BAD_RESP;
    Activity a(*session);
    a.name = params["name"];
    a.short_name = params["short_name"];
    session->commit();
    return OK_RESP;
}

string
adm_list_activities(StringMap &params)
{
    std::auto_ptr<Yb::Session> session(theApp::instance().new_session());
    AuthControl auth(new Requester(0, params, 1, SCHEDULE_ADM));
    if (!auth.session_is_valid())
        return BAD_RESP;
    ElementPtr et = new_element("activity");
    Activity::ResultSet rs = Yb::query<Activity>(*session).all();
    Activity::ResultSet::iterator i = rs.begin(), end = rs.end();
    for (; i != end; ++i)
        et->sub_element("item", i->name);
    return etree2str(et);
}

string
adm_get_schedule_items(StringMap &params)
{  
    Responsible resp(new Requester(DOWNLOAD, params, 1, SCHEDULE_ADM));
    resp.process_request();
    return resp.get_response(); 
}
*/

BaseCommand *createCommand(int commandType, const StringMap &data)
{
    BaseCommand *command;
    switch(commandType) {
        case Auth:
            command = new AuthCommand(data);
            break;
        case SessionInfo:
            command = new SessionInfoCommand(data);
            break;
        case Logout:
            command = new LogoutCommand(data);
            break;
        case UpdateSchedule:
            command = new UpdateScheduleCommand(data);
            break;
        case DownloadSchedule:
            command = new DownloadScheduleCommand(data);
            break;
        case CheckSchedule:
            command = new CheckScheduleCommand(data);
            break;
        case LastUpdateInfo:
            command = new LastUpdateInfoCommand(data);
            break;
        default:
            throw std::runtime_error("UNKNOWN_REQUEST_TYPE");
    }
    return command;
}

string
main_handler(const Yb::StringDict &content)
{
    try {
        ContentChecker contentChecker;
        if (!contentChecker.contentIsValid(content))
            throw std::runtime_error(contentChecker.err());
    
        BaseCommand *command = createCommand(contentChecker.requestType(), contentChecker.parsedParams());
        std::string response = command->execute();
        delete command;
        return response;
    }
    catch(const exception &err) {
        logger->error(err.what());
        cout << err.what() << endl;
        return "<status>EXCEPTION</status>";
    }
}

int
main()
{
    try {
        logger = theApp::instance().new_logger("main");
    }
    catch (const std::exception &ex) {
        std::cerr << "exception: " << ex.what() << "\n";
        return 1;
    }
    try {
        //int port = 19090; // TODO: read from config
        typedef std::string (*Handler)(const Yb::StringDict &);
        typedef HttpServer<Handler> ScheduleHttpServer;
        ScheduleHttpServer::HandlerMap handlers;
        handlers["/main"] = main_handler;
       /* handlers["/adm/login"] = adm_login;
        handlers["/adm/session_info"] = adm_session_info;
        handlers["/adm/logout"] = adm_logout;
        handlers["/adm/add_activity"] = adm_add_activity;
        handlers["/adm/list_activities"] = adm_list_activities;
        handlers["/adm/schedule"] = adm_get_schedule_items;
        */
        ScheduleHttpServer server(
                theApp::instance().get_settings().get_port(),
                handlers, &theApp::instance(),
                _T("text/xml"), _T("<status>NOT</status>"));
        server.serve();
    }
    catch (const std::exception &ex) {
        logger->error(string("exception: ") + ex.what());
        return 1;
    }
    return 0;
}

// vim:ts=4:sts=4:sw=4:et:
