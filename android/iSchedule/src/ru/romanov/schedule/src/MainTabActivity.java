package ru.romanov.schedule.src;

import java.util.ArrayList;

import ru.romanov.schedule.R;
import ru.romanov.schedule.adapters.CollectionPagerAdapter;
import ru.romanov.schedule.adapters.SubjectAdapter;
import ru.romanov.schedule.utils.CalendarManager;
import ru.romanov.schedule.utils.StringConstants;
import ru.romanov.schedule.utils.Subject;
import ru.romanov.schedule.utils.UpdateService;
import android.app.ActionBar;
import android.app.ActionBar.Tab;
import android.app.ActionBar.TabListener;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Bundle;
import android.os.IBinder;
import android.provider.CalendarContract;
import android.support.v4.app.FragmentActivity;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;

public class MainTabActivity extends FragmentActivity implements OnPageChangeListener, TabListener{
	private UpdateService updateService;
	private CollectionPagerAdapter collectionPagerAdapter;

	TextView lastSyncTV;
	ViewPager tabPager;
	
    private static final Uri EVENT_URI = CalendarContract.Events.CONTENT_URI;
    private static final String ACCOUNT_NAME = "buyvich@gmail.com";
    
	private ServiceConnection connection = new ServiceConnection() {
			
			@Override
			public void onServiceDisconnected(ComponentName name) {
				Log.i("ServiceConnection", "Service UpdateManager disconnected");
				updateService = null;
			}
			
			@Override
			public void onServiceConnected(ComponentName name, IBinder service) {
				UpdateService.MyBinder binder = (UpdateService.MyBinder) service;
				updateService = binder.getService();
				Log.i("ServiceConnection", "Service UpdateManager connected");
			}
	};

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.i("Activity", "MainTabActivity created");
		setContentView(R.layout.main_tab_layout);
		lastSyncTV = (TextView) findViewById(R.id.maintab_last_sync);
		tabPager = (ViewPager) findViewById(R.id.tabPager);
		
		collectionPagerAdapter = new CollectionPagerAdapter(getSupportFragmentManager());
		tabPager.setAdapter(collectionPagerAdapter);
		tabPager.setOnPageChangeListener(this);
		
		ActionBar actionBar = getActionBar();
		actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);
		
		Tab scheduleTab = actionBar.newTab();
		scheduleTab.setText(R.string.schedule);
		scheduleTab.setTabListener(this);
		actionBar.addTab(scheduleTab);
		
		Tab updatesTab = actionBar.newTab();
		updatesTab.setText(R.string.updates);
		updatesTab.setTabListener(this);
		actionBar.addTab(updatesTab);

		// TODO: get some inspiration from here:
		// http://www.vogella.com/tutorials/AndroidServices/article.html#service_backgroundprocessing
		// 8. Exercise: Define and consume local service

		// This does not suffice
//		PendingIntent pintent = PendingIntent.getService(this, 0, uintent, 0);
//		Calendar cal = Calendar.getInstance();
//		AlarmManager alarm = (AlarmManager)getSystemService(Context.ALARM_SERVICE);
//		// Start every 30 seconds
//		alarm.setRepeating(AlarmManager.RTC_WAKEUP, cal.getTimeInMillis(), 30*1000, pintent);
	}
	

	@Override
	protected void onResume() {
		super.onResume();
		String lastSync = getSharedPreferences(
				StringConstants.SCHEDULE_SHARED_PREFERENCES, MODE_PRIVATE)
				.getString(StringConstants.SHARED_LAST_SYNC_DATE, "-");
		lastSyncTV.setText(lastSync);
		
		Intent intent = new Intent(this, UpdateService.class);
		bindService(intent, connection, Context.BIND_AUTO_CREATE);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		getMenuInflater().inflate(R.menu.main_activity_menu, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Intent intent;
		switch (item.getItemId()) {
		case R.id.menu_check_updates:
            this.testCalendar();
			break;
		case R.id.menu_exit:
			AlertDialog alert = getExitAlertDialog();
			alert.show();
			//new ExitDialogFragment(question).show(getSupportFragmentManager(), tag)
			break;
		case R.id.menu_info:
			intent = new Intent(this, UserInfoDialogActivity.class);
			startActivity(intent);
			break;
		case R.id.menu_settings:
			intent = new Intent(this, MenuSettingsActivity.class);
			startActivity(intent);
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


	@Override
	public void onTabSelected(Tab tab, android.app.FragmentTransaction ft) {
		tabPager.setCurrentItem(tab.getPosition());
	}


	@Override
	public void onTabUnselected(Tab tab, android.app.FragmentTransaction ft) {
	}


	@Override
	public void onTabReselected(Tab tab, android.app.FragmentTransaction ft) {
	}


	@Override
	public void onPageScrollStateChanged(int position) {
	}


	@Override
	public void onPageScrolled(int arg0, float arg1, int arg2) {		
	}


	@Override
	public void onPageSelected(int position) {
		getActionBar().setSelectedNavigationItem(position);
	}

}


class ExitDialogFragment extends android.support.v4.app.DialogFragment
{
	private String question;
	private Activity context;
	
	public ExitDialogFragment(String question, Activity context)
	{
		this.question = question;
		this.context = context;
	}
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}

	@Override
	public Dialog onCreateDialog(Bundle savedInstanceState) {
		AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
		builder.setIcon(R.drawable.ic_launcher);
		builder.setTitle(R.string.app_name);
		builder.setMessage(this.question);
		builder.setPositiveButton(getString(R.string.dialog_yes), new DialogInterface.OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				SharedPreferences pref = context.getSharedPreferences(
						StringConstants.SCHEDULE_SHARED_PREFERENCES,
						context.MODE_PRIVATE);
				SharedPreferences.Editor editor = pref.edit();
				editor.remove(StringConstants.TOKEN);
				editor.commit();
				SharedPreferences schedule = context.getSharedPreferences(
						StringConstants.MY_SCHEDULE,
						context.MODE_PRIVATE);
				editor = schedule.edit();
				for (String key : pref.getAll().keySet()) {
					editor.remove(key);
				}
				editor.commit();
				Intent intent = new Intent(context, IScheduleActivity.class);
				startActivity(intent);
				context.finish();
				
			}
		});
		
		builder.setNegativeButton(getString(R.string.dialog_no), new DialogInterface.OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				
			}
		});
		return super.onCreateDialog(savedInstanceState);
	}

}
