package ru.mami.schedule.utils;

public abstract class StringConstants {

    public static String DEFAULT_HOST = "http://ischedule.jumpingcrab.com";
    public static String DEFAULT_PORT = "80";
    public static String API_PATH = "/api/main";
    public static String MY_URI = DEFAULT_HOST + ":" + DEFAULT_PORT + API_PATH;

    // Preference
    public static String PREF_HOST = "host";
    public static String PREF_PORT = "port";
    public static String PREF_SYNC_CALENDAR = "syncCalendar";
    public static String PREF_CALENDAR_NAME = "calendarName";

    public static String SCHEDULE_SHARED_PREFERENCES = "schedule_mami_shared";
    public static String MY_SCHEDULE = "my_schedule";
    public static String TOKEN = "token";
    public static String SHARED_LOGIN = "login";
    public static String SHARED_PASS = "pass";
    public static String SHARED_NAME = "name";
    public static String SHARED_EMAIL = "email";
    public static String SHARED_PHONE = "phone";
    public static String SHARED_LAST_SYNC_DATE = "last_sync_date";

}
