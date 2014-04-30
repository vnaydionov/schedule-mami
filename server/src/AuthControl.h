#ifndef AUTH_CONTROL__INCLUDED
#define AUTH_CONTROL__INCLUDED

#include "domain/User.h"

class AuthControl {
    static Yb::LongInt getRandom();
    static std::string md5Hash(const std::string &str);
    static Yb::LongInt checkUser(const std::string &login, const std::string &pass);
    static void clearUserSessions(Domain::UserHolder user);
    AuthControl();
public:
    static Yb::LongInt login(const std::string &login, 
                             const std::string &pass,
                             const std::string &appName="mobile");
    static bool logout(Yb::LongInt token);
    static Yb::LongInt sessionIsValid(Yb::LongInt token, const std::string &appName="mobile");
};

#endif // AUTH_CONTROL__INCLUDED

// vim:ts=4:sts=4:sw=4:et:

