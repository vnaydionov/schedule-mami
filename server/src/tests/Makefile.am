
AM_CXXFLAGS = \
	-I $(top_builddir)/src \
	$(XML_CPPFLAGS) \
	$(BOOST_CPPFLAGS) \
	$(YBORM_CXXFLAGS) \
	$(CPPUNIT_CFLAGS)

check_PROGRAMS = unit_tests

unit_tests_SOURCES = \
	test_main.cpp \
	../App.cpp \
	TestAuthControl.cpp \
	../AuthControl.cpp \
	TestContentChecker.cpp \
	../ContentChecker.cpp \
	TestScheduleControl.cpp \
	../ScheduleControl.cpp \
	../domain/User.cpp \
	../domain/Schedule.cpp \
	../domain/Activity.cpp \
	../domain/Place.cpp \
	../domain/Subject.cpp \
	../domain/ScheduleActivity.cpp \
	../domain/LoginSession.cpp \
	../domain/ScheduleDate.cpp \
	../domain/ScheduleGroup.cpp \
	../domain/StudGroup.cpp \
	../logger.cpp \
	../md5.c

unit_tests_LDFLAGS=\
	$(XML_LIBS) \
	$(BOOST_THREAD_LDFLAGS) \
	$(BOOST_THREAD_LIBS) $(BOOST_DATE_TIME_LIBS) \
	$(ODBC_LIBS) \
	$(SQLITE3_LIBS) \
	$(SOCI_LIBS) \
	$(EXECINFO_LIBS) \
	$(YBORM_LDFLAGS) \
	$(YBORM_LIBS) \
	$(CPPUNIT_LIBS)

TESTS = unit_tests

export YBORM_DBTYPE=@YBORM_DBTYPE@
export YBORM_DB=@YBORM_DB@
export YBORM_USER=@YBORM_USER@
export YBORM_PASSWD=@YBORM_PASSWD@
export YBORM_URL=@YBORM_URL@


