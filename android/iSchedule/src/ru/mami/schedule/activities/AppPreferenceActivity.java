package ru.mami.schedule.activities;

import ru.mami.schedule.fragments.AppPreferenceFragment;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;

public class AppPreferenceActivity extends FragmentActivity {

    @Override
    protected void onCreate(Bundle arg0) {
        super.onCreate(arg0);
        // can use it for 1.6+ support
        // https://github.com/kolavar/android-support-v4-preferencefragment
        getFragmentManager().beginTransaction()
            .replace(android.R.id.content, new AppPreferenceFragment()).commit();
    }

}
