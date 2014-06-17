package ru.mami.schedule.adapters;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;
import ru.mami.schedule.utils.Subject;

import java.util.ArrayList;
import java.util.HashMap;

public class SubjectAdapter extends SQLiteOpenHelper {

    public static final String DB_NAME = "schedule_db";
    private static final int DB_VERSION = 1;

    public static final String[] columns = {
        "id", "subject", "checked", "period", "dt_start", "dt_end",
        "dow", "t_start", "t_end", "groups", "place", "activities"};

    public static final String TABLE_NAME = "T_SUBJECT";
    public static final String PLACE = "place";
    public static final String CHECKED = "checked";
    public static final String SUBJECT = "subject";
    public static final String PERIOD = "period";
    public static final String DT_START = "dt_start";
    public static final String DT_END = "dt_end";
    public static final String DOW = "dow";
    public static final String T_START = "t_start";
    public static final String T_END = "t_end";
    public static final String GROUPS = "groups";
    public static final String ACTIVITIES = "activities";
    private static final String CREATE_TABLE = "create table " + TABLE_NAME +
            " ( id integer primary key, " +
            SUBJECT + " TEXT, " +
            PERIOD + " TEXT, " +
            DT_START + " TEXT, " +
            DT_END + " TEXT, " +
            DOW + " TEXT, " +
            T_START + " TEXT, " +
            T_END + " TEXT, " +
            CHECKED + " TEXT, " +
            PLACE + " TEXT, " +
            GROUPS + " TEXT, " +
            ACTIVITIES + " TEXT)";

    public SubjectAdapter(Context context) {
        super(context, DB_NAME, null, DB_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(CREATE_TABLE);
    }

    @Override
    public void onUpgrade(SQLiteDatabase arg0, int arg1, int arg2) {
    }

    public void saveSubject(Subject subj) {
        ContentValues cv = new ContentValues();
        cv.put("subject", subj.getSubject());
        cv.put("place", subj.getPlace());
        cv.put("period", subj.getPeriod());
        cv.put("dt_start", subj.getDt_start());
        cv.put("dt_end", subj.getDt_end());
        cv.put("t_start", subj.getT_start());
        cv.put("t_end", subj.getT_end());
        cv.put("groups", subj.getGroups());
        cv.put("dow", subj.getDow());
        cv.put("checked", subj.getChecked());
        cv.put("activities", subj.getActivities());

        SQLiteDatabase db = this.getWritableDatabase();
        try {
            int updCount = db.update(SubjectAdapter.TABLE_NAME, cv,
                    "id = ?", new String[] {subj.getId()});
            if (updCount == 0) {
                Log.i("saveSubject", "insert " + subj.getId());
                cv.put("id", subj.getId());
                db.insert(SubjectAdapter.TABLE_NAME, null, cv);
            }
            else {
                Log.i("saveSubject", "update " + subj.getId());
            }
        }
        finally {
            db.close();
        }
    }

    public void deleteSubject(Subject subj) {
        Log.i("deleteSubject", subj.getId());
        SQLiteDatabase db = this.getWritableDatabase();
        try {
            db.delete(SubjectAdapter.TABLE_NAME,
                    "id = ?", new String[] {subj.getId()});
        }
        finally {
            db.close();
        }
    }

    public void syncDB(ArrayList<Subject> subjects) {
        if (subjects != null) {
            for (Subject subj : subjects) {
                if (subj.getMode().equals("add"))
                    this.saveSubject(subj);
                else
                    this.deleteSubject(subj);
            }
        }
    }

    private ArrayList<Subject> loadSubjects(Cursor cursor) {
        Log.i("loadSubjects", "start");
        ArrayList<Subject> subjects = new ArrayList<Subject>();
        if (cursor != null && cursor.moveToFirst()) {
            String[] names = cursor.getColumnNames();
            do {
                HashMap<String, String> rawSubj = new HashMap<String, String>();
                Log.i("loadSubjects", "--new object");
                for (int i = 0; i < names.length; ++i) {
                    rawSubj.put(names[i], cursor.getString(i));
                    Log.i("loadSubjects", "name=" + names[i] + 
                            ", value=" + cursor.getString(i));
                }
                Subject subj = new Subject(rawSubj);
                subjects.add(subj);
            } while (cursor.moveToNext());
        }
        Log.i("loadSubjects", "--total count=" + subjects.size());
        return subjects;
    }

    public ArrayList<Subject> getNewSubjects() {
        Log.i("getNewSubjects", "start");
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.query(TABLE_NAME, columns, 
                "checked = ?", new String[] {"false"}, null, null, null);
        ArrayList<Subject> subjects = loadSubjects(cursor);
        db.close();
        return subjects;
    }

    public ArrayList<Subject> getSubjectsByDate(String day) {
        Log.i("getSubjectsByDate", day);
/*      
        HashMap<String, String> ruDays = new HashMap<String, String>();
        ruDays.put("Mon", "Понедельник");
        ruDays.put("Tue", "Вторник");
        ruDays.put("Wed", "Среда");
        ruDays.put("Thu", "Четверг");
        ruDays.put("Fri", "Пятница");
        ruDays.put("Sat", "Суббота");
        ruDays.put("Sun", "Воскресенье");
        String dow = ruDays.get(day);
*/
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.query(TABLE_NAME, columns,
                "dow = ?", new String[] {day}, null, null, null);
        ArrayList<Subject> subjects = loadSubjects(cursor);
        db.close();
        return subjects;
    }

    public Subject getSubjectById(String id) {
        Log.i("getSubjectById", id);
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.query(TABLE_NAME, columns,
                "id = ?", new String[] {id}, null, null, null);
        ArrayList<Subject> subjects = loadSubjects(cursor);
        db.close();
        if (subjects.isEmpty())
            return null;
        return subjects.get(0);
    }

    public ArrayList<Subject> getAll() {
        Log.i("getAll", "start");
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.query(true, TABLE_NAME,
                null, null, null, null, null, null, null);
        ArrayList<Subject> subjects = loadSubjects(cursor);
        db.close();
        return subjects;
    }

}
