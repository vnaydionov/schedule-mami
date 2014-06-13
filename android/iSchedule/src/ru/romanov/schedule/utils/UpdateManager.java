package ru.romanov.schedule.utils;

import ru.romanov.schedule.src.MainTabActivity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class UpdateManager extends BroadcastReceiver {
	// TODO Пересмотреть необходимость этого класса
	@Override
	public void onReceive(Context context, Intent intent) {
		Log.i(getClass().getSimpleName(), "onReceive()");
		Log.i(getClass().getSimpleName(), intent.getAction());
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

