
add_subdirectory (domain)

include_directories (
    ${ICONV_INCLUDES}
    ${LIBXML2_INCLUDES}
    ${BOOST_INCLUDES}
    ${YBORM_INCLUDES}
    ${PROJECT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_BINARY_DIR})

add_executable (schedule
    tcp_socket.cpp micro_http.cpp logger.cpp md5.c
    schedule_main.cpp ScheduleControl.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/User.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/LoginSession.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/Activity.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/ScheduleActivity.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/Schedule.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/ScheduleDate.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/StudGroup.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/ScheduleGroup.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/Subject.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/Place.cpp
    )

set_source_files_properties (
    ${CMAKE_CURRENT_BINARY_DIR}/domain/User.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/LoginSession.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/Activity.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/ScheduleActivity.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/Schedule.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/ScheduleDate.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/StudGroup.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/ScheduleGroup.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/Subject.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/domain/Place.cpp
    PROPERTIES GENERATED 1)

add_dependencies (schedule generate_domain)

if (UNIX)
target_link_libraries (schedule ${YBORM_LIB} ${YBUTIL_LIB} ${LIBXML2_LIBS}
    ${BOOST_DATE_TIME_LIBS} ${BOOST_THREAD_LIBS} ${ODBC_LIBS})
else ()
target_link_libraries (schedule ${YBORM_LIB} ${YBUTIL_LIB} ${LIBXML2_LIBS}
    ${BOOST_DATE_TIME_LIBS} ${BOOST_THREAD_LIBS} ${ODBC_LIBS}
    ${SOCKET_LIB} ${UUID_LIB})
endif ()

install (TARGETS schedule DESTINATION examples)
install (FILES schedule.bat schedule_schema.xml DESTINATION examples)

