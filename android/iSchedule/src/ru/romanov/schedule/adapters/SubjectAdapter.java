package ru.romanov.schedule.adapters;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import ru.romanov.schedule.utils.Subject;

import java.util.ArrayList;
import java.util.HashMap;

public class SubjectAdapter extends SQLiteOpenHelper {
	
	public static final String DB_NAME = "schedule_db";
	private static final int DB_VERSION = 1;
	
	public static final String[] columns = {"id", "subject", "checked", "period", "dt_start", "dt_end",
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
	private static final String CREATE_TABLE = "create table " + TABLE_NAME + " ( id integer primary key, "
		      + SUBJECT + " TEXT, " + PERIOD + " TEXT, " + DT_START + " TEXT, " + DT_END + " TEXT, " 
			+ DOW + " TEXT, " + T_START + " TEXT, " + T_END + " TEXT, " + CHECKED + " TEXT, " 
		      + PLACE + " TEXT, " + GROUPS + " TEXT, " + ACTIVITIES + " TEXT)";

	public SubjectAdapter(Context context) {
		super(context, DB_NAME, null, DB_VERSION);
		// TODO Auto-generated constructor stub
	}

	@Override
	public void onCreate(SQLiteDatabase db) {
		db.execSQL(CREATE_TABLE);
	}

	@Override
	public void onUpgrade(SQLiteDatabase arg0, int arg1, int arg2) {
		// TODO Auto-generated method stub
	}
	
	public void saveSubject(Subject subj) {
		SQLiteDatabase db = this.getWritableDatabase();
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
		cv.put("id", subj.getId());
        cv.put("activities", subj.getActivities());
		db.insert(SubjectAdapter.TABLE_NAME, null, cv);
	}
	
	public void deleteSubject(Subject subj) {
		
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

    public ArrayList<Subject> getNewSubjects() {
        ArrayList<Subject> subjects = new ArrayList<Subject>();
        HashMap<String, String> rawSubj = new HashMap<String, String>();

        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.query(TABLE_NAME, columns, "checked = ?", new String[] {"false"}, null, null, null);
        if (cursor != null) {
            if (cursor.moveToFirst()){
                String[] names = cursor.getColumnNames();
                do{
                    for (int i = 0; i < names.length; ++i) {
                        rawSubj.put(names[i], cursor.getString(i));
                    }
                    Subject subj = new Subject(rawSubj);
                    subjects.add(subj);
                }  while(cursor.moveToNext());
            }
        }
        db.close();
        return subjects;
    }
	
	public ArrayList<Subject> getSubjectsByDate(String day) {
        ArrayList<Subject> subjects = new ArrayList<Subject>();
        HashMap<String, String> rawSubj = new HashMap<String, String>();
        HashMap<String, String> ruDays = new HashMap<String, String>();
        ruDays.put("Mon", "Понедельник");
        ruDays.put("Tue", "Вторник");
        ruDays.put("Wed", "Среда");
        ruDays.put("Thu", "Четверг");
        ruDays.put("Fri", "Пятница");
        ruDays.put("Sat", "Суббота");
        ruDays.put("Sun", "Воскресенье");

        String dow = ruDays.get(day);

        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.query(TABLE_NAME, columns, "dow = ?", new String[] {dow}, null, null, null);
        if (cursor != null) {
            if (cursor.moveToFirst()){
                String[] names = cursor.getColumnNames();
                do{
                    for (int i = 0; i < names.length; ++i) {
                        rawSubj.put(names[i], cursor.getString(i));
                    }
                    Subject subj = new Subject(rawSubj);
                    subjects.add(subj);
                }  while(cursor.moveToNext());
            }
        }
        db.close();
        return subjects;
	}
	
	public Subject getSubjectById(String id) {
		return null;
	}
	
	public ArrayList<Subject> getAll() {
		ArrayList<Subject> subjects = new ArrayList<Subject>();
		HashMap<String, String> rawSubj = new HashMap<String, String>();
		SQLiteDatabase db = this.getReadableDatabase();
		Cursor cursor = db.query(true, TABLE_NAME, columns, null, null, null, null, null, null);
		if (cursor != null) {
			cursor.moveToFirst();
			String[] names = cursor.getColumnNames();
			for (int i = 0; i < names.length; ++i) {
				rawSubj.put(names[i], cursor.getString(i));
			}
			Subject subj = new Subject(rawSubj);
			subjects.add(subj);
			while (cursor.moveToNext()) {
				for (int i = 0; i < names.length; ++i) {
					rawSubj.put(names[i], cursor.getString(i));
				}
				subj = new Subject(rawSubj);
				subjects.add(subj);
			}
		}
		db.close();
		return subjects;
	}

}
