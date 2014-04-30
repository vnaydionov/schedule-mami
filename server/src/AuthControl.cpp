#if defined(__WIN32__) || defined(_WIN32)
#include <rpc.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif
#include "AuthControl.h"
#include "domain/User.h"
#include "domain/LoginSession.h"
#include "md5.h"
#include <stdio.h>
#include "App.h"

using namespace std;
using namespace Yb::ElementTree;
using namespace Yb;
using namespace Domain;

AuthControl::AuthControl()
{}

Yb::LongInt
AuthControl::getRandom()
{
    Yb::LongInt buf;
#if defined(__WIN32__) || defined(_WIN32)
    UUID new_uuid;
    UuidCreate(&new_uuid);
    buf = new_uuid.Data1;
    buf <<= 32;
    Yb::LongInt buf2 = (new_uuid.Data2 << 16) | new_uuid.Data3;
    buf += buf2;
#else
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1)
        throw runtime_error("can't open /dev/urandom");
    if (read(fd, &buf, sizeof(buf)) != sizeof(buf)) {
        close(fd);
        throw runtime_error("can't read from /dev/urandom");
    }
    close(fd);
#endif
    return buf;
}

string 
AuthControl::md5Hash(const string &str)
{
    MD5_CTX mdContext;
    MD5Init(&mdContext);
    MD5Update(&mdContext, (unsigned char *)str.c_str(), str.size());
    MD5Final(&mdContext);
    string rez;
    char omg[3];
    for (int i = 0; i < 16; ++i) {
        sprintf(omg, "%02x", mdContext.digest[i]);
        rez.append(omg, 2);
    }
    return rez;
}

Yb::LongInt
AuthControl::checkUser(const std::string &login, const std::string &pass)
{
    std::auto_ptr<Yb::Session> session = theApp::instance().new_session();
    User::ResultSet rs = Yb::query<User>(*session, User::c.login == login).all();
    User::ResultSet::iterator q = rs.begin();
    if (q == rs.end())
        return -1;
    if (q->pass != md5Hash(pass)
            //|| q->is_superuser && q->status != 0
    )
        return -1;
    return q->id;
}

void
AuthControl::clearUserSessions(UserHolder user)
{
    //Yb::ManagedList<LoginSession> ls_list = user->login_sessions;
    //Yb::ManagedList<LoginSession>::iterator i = ls_list.begin();
    LoginSession::ResultSet rs = query<LoginSession>(*user->get_session())
        .filter_by((LoginSession::c.usr_id == user->id))
        .all();
    LoginSession::ResultSet::iterator it = rs.begin();
    for (; it != rs.end(); ++it)
        it->delete_object();
    user->get_session()->commit();
}

Yb::LongInt
AuthControl::login(const std::string &login, const std::string &pass,
                    const std::string &appName)
{
    Yb::LongInt userId = checkUser(login, pass);
    if (userId == -1)
        return -1;
    
    std::auto_ptr<Yb::Session> session = theApp::instance().new_session();
    
    UserHolder user(*session, userId);
    clearUserSessions(user);
    
    LoginSession login_session(*session);
    login_session.user = user;
    Yb::LongInt token = getRandom();
    login_session.token = boost::lexical_cast<string>(token);
    login_session.end_session = Yb::now() + boost::posix_time::hours(11);
    login_session.app_name = appName;
    session->commit();
    return token;
}

bool
AuthControl::logout(Yb::LongInt token)
{
    std::auto_ptr<Yb::Session> session = theApp::instance().new_session();
    LoginSession::ResultSet rs = Yb::query<LoginSession>(*session,
            LoginSession::c.token == token).all();
    LoginSession::ResultSet::iterator q = rs.begin(), qend = rs.end();
    if (q == qend)
        return false;

    q->delete_object();
    q->get_session()->commit();
    return true;
}

Yb::LongInt
AuthControl::sessionIsValid(Yb::LongInt token, const std::string &appName)
{
    std::auto_ptr<Yb::Session> session = theApp::instance().new_session();
    //typedef boost::tuple<LoginSession, User> Row;
    Yb::DomainResultSet<LoginSession> rs = Yb::query<LoginSession>(*session)
        .filter_by(LoginSession::c.token == token).all();
                //&& LoginSession::c.app_name == appName).all();
    Yb::DomainResultSet<LoginSession>::iterator q = rs.begin(), qend=rs.end();
    if (q == qend)
        return -1;
    
    //LoginSession &ls = q->get<0>();
    // TODO
    //if (req_man->access) {
    //    if (ls.user->status != 0)
    //        return -1;
    //}
    if (Yb::now() >= q->end_session)
        return -1;
    return q->id;
}

// vim:ts=4:sts=4:sw=4:et:
