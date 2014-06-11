package ru.romanov.schedule.utils;

import java.util.Calendar;

import ru.romanov.schedule.src.MainTabActivity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.AlarmManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class UpdateManager extends BroadcastReceiver {
	private static final long REPEAT_TIME = 1000 * 10;

	@Override
	public void onReceive(Context context, Intent intent) {
		//context.startService(new Intent(context, UpdateService.class));
		Log.i("UpdateManager", "onReceive()");
		AlarmManager service = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
		Intent i = new Intent(context, UpdateSeviceReciever.class);
		PendingIntent pending = PendingIntent.getBroadcast(context, 0, i, 
		        PendingIntent.FLAG_CANCEL_CURRENT);
		Calendar cal = Calendar.getInstance();
		// start 5 seconds after boot completed
		cal.add(Calendar.SECOND, 5);
		
		// fetch every 30 seconds
	    // InexactRepeating allows Android to optimize the energy consumption
	    service.setInexactRepeating(AlarmManager.RTC_WAKEUP,
	        cal.getTimeInMillis(), REPEAT_TIME, pending);
	}
	
	public static void notificateAboutUpdate(Context context){
		String ticketText = "Доступно обновление расписания";
		long when = System.currentTimeMillis();
		int id = android.R.drawable.sym_action_email;
		
		String srvName = Context.NOTIFICATION_SERVICE;
		NotificationManager notificationManager;
		notificationManager = (NotificationManager) context.getSystemService(srvName);
		
		Notification notification = new Notification(id, ticketText, when);
		
		Intent mainIntent = new Intent(context, MainTabActivity.class);
		notification.setLatestEventInfo(context, ticketText, "",
	            PendingIntent.getActivity(context, 1, mainIntent, 0));
		
		notification.flags |= Notification.FLAG_AUTO_CANCEL;
		notificationManager.notify(0, notification);
	}
}

class UpdateSeviceReciever extends BroadcastReceiver {
	@Override
	public void onReceive(Context context, Intent arg1) {
		Intent service = new Intent(context, UpdateService.class);
	    context.startService(service);
	}
}
