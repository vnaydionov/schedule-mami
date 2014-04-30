#include <cppunit/extensions/HelperMacros.h>
#include "../App.h"
#include "../ScheduleControl.h"
#include "../domain/User.h"
#include "../domain/Schedule.h"
#include "../domain/Activity.h"
#include "../domain/Place.h"
#include "../domain/Subject.h"
#include "../domain/ScheduleActivity.h"

using namespace std;
using namespace Yb;
using namespace Domain;

class TestScheduleCreator
{
    LongInt schedule_id_;
    LongInt user_id_;
    ILogger::Ptr log_;

public:
    TestScheduleCreator()
        : schedule_id_(-1)
        , user_id_(-1)
        , log_(theApp::instance().new_logger("TestScheduleCreator"))
    {
        log_->info("constructor called");
    }

    LongInt get_schedule_id() { return schedule_id_; }
    LongInt get_user_id() { return user_id_; }

    void fill()
    {
        ILogger::Ptr log = log_->new_logger("fill");
        log->info("started");
        if (schedule_id_ != -1)
            clean();
        auto_ptr<Session> session = theApp::instance().new_session();

        log->info("create user");
        UserHolder usr(*session);
        usr->name = "test_user";
        usr->login = "test_login";
        usr->pass = "4ac1b63dca561d274c6055ebf3ed97db";

        log->info("create activity");
        ActivityHolder activity(*session);
        activity->name = "test_activity";
        activity->short_name = "t_a";

        log->info("create place");
        PlaceHolder place(*session);
        place->name = "test_place";
        place->short_name = "t_p";

        log->info("create subject");
        SubjectHolder subj(*session);
        subj->name = "test_subj";
        subj->short_name = "t_s";

        log->info("create schedule item");
        ScheduleHolder schedule(*session);
        schedule->update_dt = now(); // "2012-12-01T23:00:00"
        schedule->teacher = usr;
        schedule->receiver = usr;
        schedule->subject = subj;
        schedule->place = place;
        schedule->start_dt = now(); // "2012-10-01T23:00:00"
        schedule->end_dt = now();   // "2012-12-01T23:00:00"
        schedule->dow = "Tue";
        schedule->repeat_type = "each";
        schedule->start_time = 900;
        schedule->end_time = 1040;

        log->info("create schedule-activity");
        ScheduleActivityHolder sched_activity(*session);
        sched_activity->schedule = schedule;
        sched_activity->activity = activity;

        session->commit();
        schedule_id_ = schedule->id;
        user_id_ = usr->id;
        log->info("finished");
    }

    void clean()
    {
        ILogger::Ptr log = log_->new_logger("clean");
        log->info("started");
        if (schedule_id_ != -1) {
            log->info("delete schedule data");
            auto_ptr<Session> session = theApp::instance().new_session();
            ScheduleHolder schedule(*session, schedule_id_);
            UserHolder usr = schedule->teacher;
            PlaceHolder place = schedule->place;
            SubjectHolder subj = schedule->subject;
            ScheduleActivity sched_activity = *schedule->activities.begin();
            ActivityHolder activity = sched_activity.activity;
            //sched_activity.delete_object();
            schedule->delete_object();
            activity->delete_object();
            usr->delete_object();
            place->delete_object();
            subj->delete_object();
            session->commit();
            schedule_id_ = -1;
            user_id_ = -1;
        }
        log->info("finished");
    }

    ~TestScheduleCreator()
    {
        log_->info("destructor called");
        if (schedule_id_ != -1)
            clean();
    }
};

class TestScheduleControl: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestScheduleControl);
    CPPUNIT_TEST(testGetSchedule);
    CPPUNIT_TEST_SUITE_END(); 

    TestScheduleCreator test_data;
public:
    void setUp()
    {
        test_data.fill();
    }

    void tearDown()
    {
        test_data.clean();
    }

    void testGetSchedule()
    {
        App &app = theApp::instance();
        ILogger::Ptr log = app.new_logger("testGetSchedule");
        auto_ptr<Session> session = app.new_session();
        UserHolder user(*session, test_data.get_user_id());
        //CPPUNIT_ASSERT_EQUAL(rs.end(), rs.begin());
        string data = ScheduleControl::download(user, string("2011-05-03"))->serialize();
        log->info(data);
        CPPUNIT_ASSERT(!data.empty());
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestScheduleControl);

// vim:ts=4:sts=4:sw=4:et:
