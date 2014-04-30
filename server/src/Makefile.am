
if CPPUNIT_PRESENT
SUBDIRS=domain tests
else
SUBDIRS=domain
endif

AM_CXXFLAGS=\
	-I $(top_srcdir)/lib \
	$(XML_CPPFLAGS) \
	$(BOOST_CPPFLAGS) \
	$(YBORM_CXXFLAGS)

bin_PROGRAMS=schedule

schedule_SOURCES=\
	App.cpp \
	domain/User.cpp \
	domain/LoginSession.cpp \
	domain/Activity.cpp \
	domain/ScheduleActivity.cpp \
	domain/Schedule.cpp \
	domain/ScheduleDate.cpp \
	domain/StudGroup.cpp \
	domain/ScheduleGroup.cpp \
	domain/Subject.cpp \
	domain/Place.cpp \
	ScheduleControl.cpp \
	ContentChecker.cpp \
	AuthControl.cpp \
	schedule_main.cpp \
	tcp_socket.cpp \
	micro_http.cpp \
	logger.cpp \
	md5.c \
	Commands.cpp

schedule_LDFLAGS=\
	$(XML_LIBS) \
	$(BOOST_LDFLAGS) \
	$(BOOST_LIBS_R) \
	$(ODBC_LIBS) \
	$(DBPOOL3_LDFLAGS) \
	$(DBPOOL3_LIBS) \
	$(YBORM_LDFLAGS) \
	$(YBORM_LIBS)

