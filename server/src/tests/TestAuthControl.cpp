#include <cppunit/extensions/HelperMacros.h>
#include "../App.h"
#include "../AuthControl.h"
#include "../domain/User.h"
#include "../domain/LoginSession.h"

using namespace std;
using namespace Yb;
using namespace Domain;

class TestUserCreator 
{
    LongInt user_id_;
    Yb::ILogger::Ptr log_;

public:
    TestUserCreator()
        : user_id_(-1)
        , log_(theApp::instance().new_logger("TestUserCreator"))
    {
        log_->info("constructor called");
    }

    LongInt get_user_id() { return user_id_; }

    void fill()
    {
        Yb::ILogger::Ptr log = log_->new_logger("fill");
        log->info("started");
        if (user_id_ != -1)
            clean();
        auto_ptr<Session> session = theApp::instance().new_session();
        log->info("create user");
        UserHolder usr(*session);
        usr->name = "test_user";
        usr->login = "test_login";
        usr->pass = "4ac1b63dca561d274c6055ebf3ed97db";
        session->commit();
        user_id_ = usr->id;
        log->info("finished");
    }

    void clean() 
    {
        Yb::ILogger::Ptr log = log_->new_logger("clean");
        log->info("started");
        if (user_id_ != -1) {
            log->info("delete user");
            auto_ptr<Session> session = theApp::instance().new_session();
            UserHolder usr(*session, user_id_);
            while (usr->login_sessions.begin() != usr->login_sessions.end())
                usr->login_sessions.begin()->delete_object();       
            usr->delete_object();
            session->commit();
            user_id_ = -1;
        }
        log->info("finished");
    }

    ~TestUserCreator() {
        log_->info("destructor called");
        if (user_id_ != -1)
            clean();
    }
};

class TestAuthControl: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestAuthControl);
    CPPUNIT_TEST(testLogin);
    CPPUNIT_TEST(testLogout);
    CPPUNIT_TEST(testCheckAuthData);
    CPPUNIT_TEST(testCheckSession);
    CPPUNIT_TEST_SUITE_END(); 

    TestUserCreator test_data;
    Yb::ILogger::Ptr log_;
public:
    TestAuthControl()
        : log_(theApp::instance().new_logger("TestAuthControl"))
    {}

    void setUp()
    {
        test_data.fill();
    }

    void tearDown()
    {
        test_data.clean();
    }
    
    bool check_user_session()
    {
        auto_ptr<Session> session = theApp::instance().new_session();
        return Yb::query<LoginSession>(*session)
            .filter_by(LoginSession::c.usr_id == test_data.get_user_id()).count() != 0;
    }

    void testLogin()
    {
        Yb::ILogger::Ptr log = log_->new_logger("testLogin");
        log->info("started");
        Yb::LongInt token = AuthControl::login("test_login", "test_pass");
        //CPPUNIT_ASSERT_EQUAL(true, check_user_session()); 
        //CPPUNIT_ASSERT_NOT_EQUAL(-1, token); 
        log->info("finished");
    }

    void testLogout()
    {
        Yb::ILogger::Ptr log = log_->new_logger("testLogout");
        log->info("started");
        
        auto_ptr<Session> session = theApp::instance().new_session();
        UserHolder usr(*session, test_data.get_user_id());
        LoginSession login_session(*session);
        login_session.user = usr;
        login_session.token = "546239437621894389";
        login_session.app_name = "auth";
        session->commit();
        CPPUNIT_ASSERT_EQUAL(true, check_user_session());
        // Bad token check
        bool success = AuthControl::logout(434677324);
        CPPUNIT_ASSERT_EQUAL(false, success);
        // Good token check
        //success = AuthControl::logout(login_session.token.value());
        //CPPUNIT_ASSERT_EQUAL(true, success);
        log->info("finished");
    }

    void testCheckAuthData()
    { 
        Yb::ILogger::Ptr log = log_->new_logger("testCheckAuthData");
        log->info("started");
        Yb::LongInt token = AuthControl::login("bad_login", "bad_pass");
        CPPUNIT_ASSERT_EQUAL(Yb::LongInt(-1), token);
        log->info("finished");
    }

    void testCheckSession()
    {  
        Yb::ILogger::Ptr log = log_->new_logger("testCheckSession");
        log->info("started");
        
        auto_ptr<Session> session = theApp::instance().new_session();
        UserHolder usr(*session, test_data.get_user_id());
        LoginSession login_session(*session);
        login_session.user = usr;
        login_session.token = "12345";
        login_session.app_name = "mobile_agent";
        login_session.end_session = Yb::now() + boost::posix_time::hours(11);
        session->commit();
        Yb::LongInt sessionId; // = AuthControl::sessionIsValid(login_session.token.value().as_longint());
        //CPPUNIT_ASSERT_NOT_EQUAL(-1, sessionId);
        sessionId = AuthControl::sessionIsValid(Yb::LongInt(54321));
        CPPUNIT_ASSERT_EQUAL(Yb::LongInt(-1), sessionId);
        log->info("finished");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestAuthControl);

// vim:ts=4:sts=4:sw=4:et:
