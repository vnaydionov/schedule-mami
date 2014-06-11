package ru.romanov.schedule.utils;

import ru.romanov.schedule.src.MainTabActivity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.AlarmManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class UpdateManager extends BroadcastReceiver {
	private static final long REPEAT_TIME = 1000 * 10;

	@Override
	public void onReceive(Context context, Intent intent) {
		//context.startService(new Intent(context, UpdateService.class));
		//AlarmManager
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
