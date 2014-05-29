
AC_INIT([schedule-server], [0.2])
AC_CONFIG_SRCDIR([src/schedule_main.cpp])
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE

AC_LANG_CPLUSPLUS
AC_PROG_CXX
AM_PROG_LIBTOOL

AM_PATH_XML2
CPPFLAGS="$CPPFLAGS $XML_CPPFLAGS"
BOOST_REQUIRE([1.31.0],
    AC_MSG_ERROR([Package requires the Boost C++ libraries!]))
BOOST_THREADS([mt])
BOOST_DATE_TIME([mt])
CPPFLAGS="$CPPFLAGS -DYB_USE_TUPLE"

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

YB_EXECINFO([], [])

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
