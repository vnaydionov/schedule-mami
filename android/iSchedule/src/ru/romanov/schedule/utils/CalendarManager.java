package ru.romanov.schedule.utils;

import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.SharedPreferences;
import android.net.Uri;
import android.provider.CalendarContract;
import android.provider.CalendarContract.Events;

import java.util.Calendar;
import java.util.TimeZone;

/**
 * Created with IntelliJ IDEA.
 * User: nikita
 * Date: 19.05.13
 * Time: 17:15
 * To change this template use File | Settings | File Templates.
 */
public class CalendarManager {
    private static final Uri EVENT_URI = CalendarContract.Events.CONTENT_URI;
    private Context context;
    private String accountName;
    private long calID;

    public CalendarManager(Context context) {
        this.context = context;
        SharedPreferences mSharedPreferences = context.getSharedPreferences(
                StringConstants.SCHEDULE_SHARED_PREFERENCES, context.MODE_PRIVATE);
        accountName = mSharedPreferences.getString("accountName", "");
        calID = mSharedPreferences.getLong("calendarID", 0);

    }

    public void addEvent(String title, String location, String dt_start, String dt_end,
                         String t_start, String t_end, String period) {
        ContentResolver cr = context.getContentResolver();
        ContentValues cv = new ContentValues();

        cv.put(CalendarContract.Events.CALENDAR_ID, calID);
        cv.put(Events.TITLE, title);
        cv.put(CalendarContract.Events.EVENT_LOCATION, location);
        cv.put(CalendarContract.Events.EVENT_TIMEZONE, TimeZone.getDefault().getDisplayName());

        String[] dStart = dt_start.split("-");
        String[] dEnd = dt_end.split("-");
        Integer year = Integer.parseInt(dStart[0]);
        Integer month =  Integer.parseInt(dStart[1])-1;
        Integer day = Integer.parseInt(dStart[2])-1;
        Calendar beginTime = Calendar.getInstance();

        beginTime.set(year
                , month
                , day, 7, 30);
        Calendar endTime = Calendar.getInstance();
        endTime.set(year
                , month
                , day, 8, 30);

        int count = 0;
        if(period != null && period.equals("each")) {
            for(beginTime.add(Calendar.DAY_OF_WEEK, 7); beginTime.compareTo(endTime) <= 0; beginTime.add(Calendar.DAY_OF_WEEK, 7)) {
                  count++;
            }
            String rule = "FREQ=WEEKLY;COUNT=";
            cv.put(Events.RRULE, rule + String.valueOf(count));

        }

        cv.put(Events.DTSTART, beginTime.getTimeInMillis());
        cv.put(Events.DTEND, endTime.getTimeInMillis());


        Uri uri = cr.insert(buildEventUri(), cv);
    }

    public void deleteEvent() {

    }

    public void updateEvent() {

    }

    private Uri buildEventUri() {
        return EVENT_URI
                .buildUpon()
                .appendQueryParameter(CalendarContract.CALLER_IS_SYNCADAPTER, "true")
                .appendQueryParameter(CalendarContract.Calendars.ACCOUNT_NAME, accountName)
                .appendQueryParameter(CalendarContract.Calendars.ACCOUNT_TYPE,
                        CalendarContract.ACCOUNT_TYPE_LOCAL)
                .build();
    }
}
