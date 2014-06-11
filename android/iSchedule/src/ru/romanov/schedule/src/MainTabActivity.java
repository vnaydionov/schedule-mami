package ru.romanov.schedule.src;

import android.app.AlertDialog;
import android.app.TabActivity;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.net.Uri;
import android.os.Bundle;
import android.provider.CalendarContract;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.TabHost;
import android.widget.TextView;
import ru.romanov.schedule.R;
import ru.romanov.schedule.adapters.SubjectAdapter;
import ru.romanov.schedule.utils.CalendarManager;
import ru.romanov.schedule.utils.StringConstants;
import ru.romanov.schedule.utils.Subject;

import java.util.ArrayList;

public class MainTabActivity extends TabActivity {

	TextView lastSyncTV;
    private static final Uri EVENT_URI = CalendarContract.Events.CONTENT_URI;
    private static final String ACCOUNT_NAME = "buyvich@gmail.com";

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main_tab_layout);
		lastSyncTV = (TextView) findViewById(R.id.maintab_last_sync);
		Resources res = getResources(); // Resource object to get Drawables
		TabHost tabHost = getTabHost(); // The activity TabHost
		TabHost.TabSpec spec; // Resusable TabSpec for each tab
		Intent intent; // Reusable Intent for each tab
		
		//startService(new Intent(this, UpdateService.class));

		// Create an Intent to launch an Activity for the tab (to be reused)
		intent = new Intent().setClass(this, ScheduleListActivity.class);

		// Initialize a TabSpec for each tab and add it toresId the TabHost
		spec = tabHost.newTabSpec("scedule").setIndicator(getString(R.string.schedule))
				.setContent(intent);
		tabHost.addTab(spec);

		// Do the same for the other tabs
		intent = new Intent().setClass(this, UpdateListActivity.class);
		spec = tabHost.newTabSpec("updates").setIndicator(getString(R.string.updates))
				.setContent(intent);
		tabHost.addTab(spec);

		tabHost.setCurrentTab(0);
		
	}

	@Override
	protected void onResume() {
		super.onResume();
		String lastSync = getSharedPreferences(
				StringConstants.SCHEDULE_SHARED_PREFERENCES, MODE_PRIVATE)
				.getString(StringConstants.SHARED_LAST_SYNC_DATE, "-");
		lastSyncTV.setText(lastSync);

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// TODO Auto-generated method stub
		super.onCreateOptionsMenu(menu);
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.main_menu, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Intent intent;
		switch (item.getItemId()) {
		case R.id.menu_check:

            testCalendar();
			break;
		case R.id.menu_exit:
			AlertDialog alert = getExitAlertDialog();
			alert.show();
			break;
		case R.id.menu_info:
			intent = new Intent(this, UserInfoDialogActivity.class);
			startActivity(intent);
			break;
		case R.id.menu_settings:
			intent = new Intent(this, MenuSettingsActivity.class);
			startActivity(intent);
			break;
		default:
			break;
		}

		return true;
	}

    private void testCalendar() {
        SubjectAdapter subjectAdapter = new SubjectAdapter(this);
        CalendarManager calendarManager = new CalendarManager(this);
        ArrayList<Subject> subjects = subjectAdapter.getAll();
        for (Subject subject : subjects) {
            calendarManager.addEvent(subject.getSubject()
            , subject.getPlace()
            , subject.getDt_start()
            , subject.getDt_end()
            , subject.getT_start()
            , subject.getT_end()
            , subject.getPeriod());
        }


    }

    /**Builds the Uri for events (as a Sync Adapter)*/
    public static Uri buildEventUri() {
        return EVENT_URI
                .buildUpon()
                .appendQueryParameter(CalendarContract.CALLER_IS_SYNCADAPTER, "true")
                .appendQueryParameter(CalendarContract.Calendars.ACCOUNT_NAME, ACCOUNT_NAME)
                .appendQueryParameter(CalendarContract.Calendars.ACCOUNT_TYPE,
                        CalendarContract.ACCOUNT_TYPE_LOCAL)
                .build();
    }

	private AlertDialog getExitAlertDialog() {
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setMessage(getString(R.string.sure_to_exit))
				.setCancelable(false)
				.setPositiveButton(getString(R.string.dialog_yes),
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								SharedPreferences pref = getSharedPreferences(
										StringConstants.SCHEDULE_SHARED_PREFERENCES,
										MODE_PRIVATE);
								SharedPreferences.Editor editor = pref.edit();
								editor.remove(StringConstants.TOKEN);
								editor.commit();
								SharedPreferences schedule = getSharedPreferences(
										StringConstants.MY_SCHEDULE,
										MODE_PRIVATE);
								editor = schedule.edit();
								for (String key : pref.getAll().keySet()) {
									editor.remove(key);
								}
								editor.commit();
								Intent intent = new Intent(MainTabActivity.this, IScheduleActivity.class);
								startActivity(intent);
								MainTabActivity.this.finish();
							}
						})
				.setNegativeButton(getString(R.string.dialog_no),
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								dialog.cancel();
							}
						});
		return builder.create();
	}

}
