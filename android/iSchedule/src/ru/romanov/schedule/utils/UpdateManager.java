package ru.romanov.schedule.utils;

import ru.romanov.schedule.activities.MainTabActivity;
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
        int id = android.R.drawable.sym_action_email;
        // TODO use resource string
        String ticketText = "Доступно обновление расписания";
        long when = System.currentTimeMillis();
        Notification notification = new Notification(id, ticketText, when);

        Intent mainIntent = new Intent(context, MainTabActivity.class);
        notification.setLatestEventInfo(context, ticketText, "",
                PendingIntent.getActivity(context, 1, mainIntent, 0));

        notification.flags |= Notification.FLAG_AUTO_CANCEL;

        String srvName = Context.NOTIFICATION_SERVICE;
        NotificationManager notificationManager =
            (NotificationManager) context.getSystemService(srvName);
        notificationManager.notify(0, notification);
    }
}

