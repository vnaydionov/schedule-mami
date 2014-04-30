package ru.romanov.schedule.src;


import android.accounts.Account;
import android.accounts.AccountManager;
import android.app.Activity;
import android.content.ContentResolver;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.CalendarContract.Calendars;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.*;
import ru.romanov.schedule.R;
import ru.romanov.schedule.utils.StringConstants;

import java.util.*;


public class MenuSettingsActivity extends Activity implements OnClickListener {

	private String host;
	private String port;
    private boolean isChecked;
    private String calendarName;
    private static Map<String, Long> calendars = new HashMap<String, Long>();
	SharedPreferences mSharedPreferences;
	static EditText hostText;
	static EditText portText;
    static CheckBox checkBox;
    static Spinner spinner;
	Button saveButton;
	 
	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		finish();
	}
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.settings_menu);

        int position = 0;
        int count = 0;

        mSharedPreferences = getSharedPreferences(
                StringConstants.SCHEDULE_SHARED_PREFERENCES, MODE_PRIVATE);

        getCalendars();


		this.host = mSharedPreferences.getString("host", "");
		this.port = mSharedPreferences.getString("port", "");
        this.isChecked = mSharedPreferences.getBoolean("syncCalendar", false);
        this.calendarName = mSharedPreferences.getString("calendarName", null);
        if (calendarName == null)
            calendarName = (String) calendars.keySet().toArray()[0];

        ArrayList<String> a = new ArrayList<String>();

        for (Map.Entry<String, Long> c : calendars.entrySet()) {
            a.add(c.getKey());
            if (!calendarName.equals(c.getKey()))
                position = count;
            count = count + 1;
        }

        spinner = (Spinner) findViewById(R.id.accountSpinner);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, a);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setSelection(position);
        spinner.setAdapter(adapter);

		hostText = (EditText) findViewById(R.id.hostText);
		portText = (EditText) findViewById(R.id.portText);
        checkBox = (CheckBox) findViewById(R.id.syncCheckBox);
		saveButton = (Button) findViewById(R.id.saveSettingsBtn);
	}
	
	@Override
	protected void onStart() {
		hostText.setText(host);
		portText.setText(port);
        checkBox.setChecked(isChecked);
		
		saveButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				Editor editor = mSharedPreferences.edit();
				editor.putString("host", hostText.getText().toString());
				editor.putString("port", portText.getText().toString());
                editor.putBoolean("syncCalendar", checkBox.isChecked());
                editor.putString("calendarName", spinner.getSelectedItem().toString());
                editor.putLong("calendarID", calendars.get(spinner.getSelectedItem().toString()));

				editor.commit();
				finish();
			}
		});
		
		super.onStart();
	}

    private String getAccountName() {
        String name = null;
        AccountManager manager = AccountManager.get(this);
        Account[] accounts = manager.getAccountsByType("com.google");
        List<String> possibleEmails = new LinkedList<String>();

        for (Account account : accounts) {
            possibleEmails.add(account.name);
        }

        if (!possibleEmails.isEmpty() && possibleEmails.get(0) != null)
            return possibleEmails.get(0);

        return name;
    }

    private Map<String, Long> getCalendars() {
        Cursor cur = null;
        String accountName = getAccountName();
        ContentResolver cr = getContentResolver();

        Editor editor = mSharedPreferences.edit();
        editor.putString("accountName", accountName);
        editor.commit();

        String[] eventProjection = new String[] {
                Calendars._ID,
                Calendars.CALENDAR_DISPLAY_NAME,
        };

        Uri uri = Calendars.CONTENT_URI;
        String selection = "((" + Calendars.ACCOUNT_NAME + " = ?) AND ("
                + Calendars.ACCOUNT_TYPE + " = ?) AND ("
                + Calendars.OWNER_ACCOUNT + " = ?))";
        String[] selectionArgs = new String[] {accountName, "com.google",
                accountName};


        cur = cr.query(uri, eventProjection, selection, selectionArgs, null);
        if (cur.moveToFirst()) {
            do {
                long cID = cur.getLong(0);
                String cName = cur.getString(1);
                calendars.put(cName, cID);
            } while(cur.moveToNext());
        }

        return calendars;
    }

}
