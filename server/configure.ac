AC_INIT(src/schedule_main.cpp)

AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE(schedule, 0.1)

AC_LANG_CPLUSPLUS
AC_PROG_CXX
AM_PROG_LIBTOOL

AM_PATH_XML2
CPPFLAGS="$CPPFLAGS $XML_CPPFLAGS"

# required dependencies
YB_BOOST([1.31.0],[],
    AC_MSG_ERROR([Package requires the Boost C++ libraries!]))
YB_BOOST_THREAD([],
    AC_MSG_ERROR([Package requires the Boost threads library!]))
YB_BOOST_DATETIME([],
    AC_MSG_ERROR([Package requires the Boost date_time library!]))

# optional dependencies
YB_ODBC([
    CPPFLAGS="$CPPFLAGS -DYB_USE_ODBC"
], [])
YB_SQLITE3([
    CPPFLAGS="$CPPFLAGS -DYB_USE_SQLITE3"
], [])
YB_SOCI([
    CPPFLAGS="$CPPFLAGS -DYB_USE_SOCI"
], [])
AM_CONDITIONAL([ODBC_PRESENT], [test x$have_odbc = xyes])
AM_CONDITIONAL([SQLITE3_PRESENT], [test x$have_sqlite3 = xyes])
AM_CONDITIONAL([SOCI_PRESENT], [test x$have_soci = xyes])

YB_CHECK_YBORM([],
    AC_MSG_ERROR([YB.ORM library is required!]))

YB_PATH_CPPUNIT([1.10.0],[],
                [echo "WARNING!! You will not be able to run tests!!"])
AM_CONDITIONAL([CPPUNIT_PRESENT], [test x$no_cppunit != xyes])

YB_TEST_DB

AC_OUTPUT(Makefile src/Makefile
         src/domain/Makefile src/tests/Makefile
         src/settings.xml src/tests/settings.xml)

# vim:ts=4:sts=4:sw=4:et:
